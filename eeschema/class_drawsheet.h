/********************************************/
/* Definitions for the EESchema program:    */
/********************************************/

#ifndef CLASS_DRAWSHEET_H
#define CLASS_DRAWSHEET_H

#include "base_struct.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>


extern SCH_SHEET* g_RootSheet;

/**
 * Pin (label) used in sheets to create hierarchical schematics.
 *
 * A SCH_SHEET_PIN is used to create a hierarchical sheet in the same way a
 * pin is used in a component.  It connects the objects in the sheet object
 * to the objects in the schematic page to the objects in the page that is
 * represented by the sheet.  In a sheet object, a SCH_SHEET_PIN must be
 * connected to a wire, bus, or label.  In the schematic page represented by
 * the sheet, it corresponds to a hierarchical label.
 */
class SCH_SHEET_PIN : public SCH_ITEM, public EDA_TextStruct
{
private:
    int  m_Number;      ///< Label number use for saving sheet label to file.
                        ///< Sheet label numbering begins at 2.
                        ///< 0 is reserved for the sheet name.
                        ///< 1 is reserve for the sheet file name.

public:
    int  m_Edge, m_Shape;
    bool m_IsDangling;  // TRUE non connected

public:
    SCH_SHEET_PIN( SCH_SHEET* parent,
                   const wxPoint& pos = wxPoint( 0, 0 ),
                   const wxString& text = wxEmptyString );

    ~SCH_SHEET_PIN() { }

    virtual wxString GetClass() const
    {
        return wxT( "SCH_SHEET_PIN" );
    }

    bool operator==( const SCH_SHEET_PIN* aPin ) const;

    SCH_SHEET_PIN* GenCopy();

    SCH_SHEET_PIN* Next()
    {
        return ( SCH_SHEET_PIN*) Pnext;
    }

    /**
     * Get the sheet label number.
     *
     * @return Number of the sheet label.
     */
    int GetNumber() { return m_Number; }

    /**
     * Set the sheet label number.
     *
     * @param aNumber - New sheet number label.
     */
    void SetNumber( int aNumber );

    /**
     * Get the parent sheet object of this sheet pin.
     *
     * @return The sheet that is the parent of this sheet pin or NULL if it does
     *         not have a parent.
     */
    SCH_SHEET* GetParent() const { return (SCH_SHEET*) m_Parent; }

    void Place( WinEDA_SchematicFrame* frame, wxDC* DC );

    void Draw( WinEDA_DrawPanel* panel,
               wxDC*             DC,
               const wxPoint&    offset,
               int               draw_mode,
               int               Color = -1 );

    /**
     * Plot this sheet pin object to aPlotter.
     *
     * @param aPlotter - The plotter object to plot to.
     */
    void Plot( PLOTTER* aPlotter );

    /**
     * Function Save
     * writes the data structures for this object out to a FILE in "*.sch"
     * format.
     * @param aFile The FILE to write to.
     * @return bool - true if success writing else false.
     */
    bool        Save( FILE* aFile ) const;

#if defined(DEBUG)

    // comment inherited by Doxygen from Base_Struct
    void        Show( int nestLevel, std::ostream& os );

#endif

    /** Function GetPenSize
     * @return the size of the "pen" that be used to draw or plot this item
     */
    virtual int GetPenSize();

    /** function CreateGraphicShape
     * Calculates the graphic shape (a polygon) associated to the text
     * @param aCorner_list = list to fill with polygon corners coordinates
     * @param Pos = Position of the shape
     */
    void        CreateGraphicShape( std::vector <wxPoint>& aCorner_list,
                                    const wxPoint&         Pos );

    // Geometric transforms (used in block operations):

    /** virtual function Move
     * move item to a new position.
     * @param aMoveVector = the displacement vector
     */
    virtual void Move( const wxPoint& aMoveVector )
    {
        m_Pos += aMoveVector;
    }


    /** virtual function Mirror_Y
     * mirror item relative to an Y axis
     * @param aYaxis_position = the y axis position
     */
    virtual void Mirror_Y( int aYaxis_position )
    {
        m_Edge   = m_Edge ? 0 : 1;
        m_Pos.x -= aYaxis_position;
        NEGATE(  m_Pos.x );
        m_Pos.x += aYaxis_position;
    }

    /**
     * Compare schematic sheet entry (pin?) name against search string.
     *
     * @param aSearchData - Criteria to search against.
     * @return True if this item matches the search criteria.
     */
    virtual bool Matches( wxFindReplaceData& aSearchData );
};


typedef boost::ptr_vector< SCH_SHEET_PIN > SCH_SHEET_PIN_LIST;


/* class SCH_SHEET
 * This class is the sheet symbol placed in a schematic, and is the entry point
 * for a sub schematic
 */

class SCH_SHEET : public SCH_ITEM
{
public:
    wxString m_SheetName;               /* this is equivalent to C101 for
                                         * components: it is stored in F0 ...
                                         * of the file. */
private:
    wxString m_FileName;                /* also in SCH_SCREEN (redundant),
                                         * but need it here for loading after
                                         * reading the sheet description from
                                         * file. */

protected:
    SCH_SHEET_PIN_LIST m_labels;        /* List of points to be connected.*/

public:
    int         m_SheetNameSize;        /* Size (height) of the text, used to
                                         * draw the sheet name */
    int         m_FileNameSize;         /* Size (height) of the text, used to
                                         * draw the file name */
    wxPoint     m_Pos;
    wxSize      m_Size;                 /* Position and Size of *sheet symbol */
    int         m_Layer;
    SCH_SCREEN* m_AssociatedScreen;     /* Associated Screen which
                                         * handle the physical data
                                         * In complex hierarchies we
                                         * can have many SCH_SHEET
                                         * using the same data
                                         */

public:
    SCH_SHEET( const wxPoint& pos = wxPoint( 0, 0 ) );
    ~SCH_SHEET();
    virtual wxString GetClass() const
    {
        return wxT( "SCH_SHEET" );
    }


    /**
     * Function Save
     * writes the data structures for this object out to a FILE in "*.sch"
     * format.
     * @param aFile The FILE to write to.
     * @return bool - true if success writing else false.
     */
    bool         Save( FILE* aFile ) const;

    void         Place( WinEDA_SchematicFrame* frame, wxDC* DC );
    SCH_SHEET*   GenCopy();
    void         DisplayInfo( WinEDA_DrawFrame* frame );

    /**
     * Add aLabel to this sheet.
     *
     * Note: Once a label is added to the sheet, it is owned by the sheet.
     *       Do not delete the label object or you will likely get a segfault
     *       when this sheet is destroyed.
     *
     * @param aLabel - The label to add to the sheet.
     */
    void AddLabel( SCH_SHEET_PIN* aLabel );

    SCH_SHEET_PIN_LIST& GetSheetPins() { return m_labels; }

    /**
     * Remove a sheet label from this sheet.
     *
     * @param aSheetLabel - The sheet label to remove from the list.
     */
    void RemoveLabel( SCH_SHEET_PIN* aSheetLabel );

    /**
     * Delete sheet label which do not have a corresponding hierarchical label.
     *
     * Note: Make sure you save a copy of the sheet in the undo list before calling
     *       CleanupSheet() otherwise any unrefernced sheet labels will be lost.
     */
    void CleanupSheet();

    /**
     * Return the label found at aPosition in this sheet.
     *
     * @param aPosition - The position to check for a label.
     *
     * @return The label found at aPosition or NULL if no label is found.
     */
    SCH_SHEET_PIN* GetLabel( const wxPoint& aPosition );

    /**
     * Checks if a label already exists with aName.
     *
     * @param aName - Name of label to search for.
     *
     * @return - True if label found, otherwise false.
     */
    bool HasLabel( const wxString& aName );

    bool HasLabels() { return !m_labels.empty(); }

    /**
     * Check all sheet labels against schematic for undefined hierarchical labels.
     *
     * @return True if there are any undefined labels.
     */
    bool HasUndefinedLabels();

    /** Function GetPenSize
     * @return the size of the "pen" that be used to draw or plot this item
     */
    virtual int  GetPenSize();

    /** Function Draw
     *  Draw the hierarchical sheet shape
     *  @param aPanel = the current DrawPanel
     *  @param aDc = the current Device Context
     *  @param aOffset = draw offset (usually wxPoint(0,0))
     *  @param aDrawMode = draw mode
     *  @param aColor = color used to draw sheet. Usually -1 to use the normal
     * color for sheet items
     */
    void         Draw( WinEDA_DrawPanel* aPanel,
                       wxDC*             aDC,
                       const wxPoint&    aOffset,
                       int               aDrawMode,
                       int               aColor = -1 );

    /** Function HitTest
     * @return true if the point aPosRef is within item area
     * @param aPosRef = a wxPoint to test
     */
    bool     HitTest( const wxPoint& aPosRef );

    /** Function GetBoundingBox
     *  @return an EDA_Rect giving the bounding box of the sheet
     */
    EDA_Rect GetBoundingBox();

    void     SwapData( SCH_SHEET* copyitem );

    /** Function ComponentCount
     *  count our own components, without the power components.
     *  @return the component count.
     */
    int      ComponentCount();

    /** Function Load.
     *  for the sheet: load the file m_FileName
     *  if a screen already exists, the file is already read.
     *  m_AssociatedScreen point on the screen, and its m_RefCount is
     * incremented
     *  else creates a new associated screen and load the data file.
     *  @param aFrame = a WinEDA_SchematicFrame pointer to the maim schematic
     * frame
     *  @return true if OK
     */
    bool     Load( WinEDA_SchematicFrame* aFrame );

    /** Function SearchHierarchy
     *  search the existing hierarchy for an instance of screen "FileName".
     *  @param aFilename = the filename to find
     *  @param aFilename = a location to return a pointer to the screen (if
     * found)
     *  @return bool if found, and a pointer to the screen
     */
    bool     SearchHierarchy( wxString aFilename, SCH_SCREEN** aScreen );

    /** Function LocatePathOfScreen
     *  search the existing hierarchy for an instance of screen "FileName".
     *  don't bother looking at the root sheet - it must be unique,
     *  no other references to its m_AssociatedScreen otherwise there would be
     *  loops
     *  in the hierarchy.
     *  @param  aScreen = the SCH_SCREEN* screen that we search for
     *  @param aList = the SCH_SHEET_PATH*  that must be used
     *  @return true if found
     */
    bool     LocatePathOfScreen( SCH_SCREEN*     aScreen,
                                 SCH_SHEET_PATH* aList );

    /** Function CountSheets
     * calculates the number of sheets found in "this"
     * this number includes the full subsheets count
     * @return the full count of sheets+subsheets contained by "this"
     */
    int      CountSheets();

    /** Function GetFileName
     * return the filename corresponding to this sheet
     * @return a wxString containing the filename
     */
    wxString GetFileName( void );

    // Set a new filename without changing anything else
    void SetFileName( const wxString& aFilename )
    {
        m_FileName = aFilename;
    }


    /** Function ChangeFileName
     * Set a new filename and manage data and associated screen
     * The main difficulty is the filename change in a complex hierarchy.
     * - if new filename is not already used: change to the new name (and if an
     *   existing file is found, load it on request)
     * - if new filename is already used (a complex hierarchy) : reference the
     *   sheet.
     * @param aFileName = the new filename
     * @param aFrame = the schematic frame
     */
    bool ChangeFileName( WinEDA_SchematicFrame* aFrame,
                         const wxString&        aFileName );

    //void      RemoveSheet(SCH_SHEET* sheet);
    //to remove a sheet, just delete it
    //-- the destructor should take care of everything else.

    // Geometric transforms (used in block operations):

    /** virtual function Move
     * move item to a new position.
     * @param aMoveVector = the displacement vector
     */
    virtual void Move( const wxPoint& aMoveVector )
    {
        m_Pos += aMoveVector;
        BOOST_FOREACH( SCH_SHEET_PIN& label, m_labels )
        {
            label.Move( aMoveVector );
        }
    }


    /** virtual function Mirror_Y
     * mirror item relative to an Y axis
     * @param aYaxis_position = the y axis position
     */
    virtual void Mirror_Y( int aYaxis_position );

    /**
     * Compare schematic sheet file and sheet name against search string.
     *
     * @param aSearchData - Criteria to search against.
     *
     * @return True if this item matches the search criteria.
     */
    virtual bool Matches( wxFindReplaceData& aSearchData );

    /**
     * Resize this sheet to aSize and adjust all of the labels accordingly.
     *
     * @param aSize - The new size for this sheet.
     */
    void Resize( const wxSize& aSize );

#if defined(DEBUG)

    // comment inherited by Doxygen from Base_Struct
    void         Show( int nestLevel, std::ostream& os );

#endif

protected:

    /**
     * Renumber labels in list.
     *
     * This method is used internally by SCH_SHEET to update the label numbering
     * when the label list changes.  Make sure you call this method any time a
     * label is added or removed.
     */
    void renumberLabels();
};

#endif /* CLASS_DRAWSHEET_H */
