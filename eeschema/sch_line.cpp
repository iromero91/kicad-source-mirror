/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2015 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 1992-2017 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file sch_line.cpp
 * @brief Class SCH_LINE implementation
 */

#include <fctsys.h>
#include <gr_basic.h>
#include <macros.h>
#include <class_drawpanel.h>
#include <class_plotter.h>
#include <base_units.h>
#include <eeschema_config.h>
#include <general.h>
#include <protos.h>
#include <sch_line.h>
#include <schframe.h>
#include <class_netlist_object.h>

#include <dialogs/dialog_edit_line_style.h>

const enum wxPenStyle SCH_LINE::PenStyle[] =
{
        [PLOTDASHTYPE_SOLID] = wxPENSTYLE_SOLID,
        [PLOTDASHTYPE_DASH] = wxPENSTYLE_SHORT_DASH,
        [PLOTDASHTYPE_DOT] = wxPENSTYLE_DOT,
        [PLOTDASHTYPE_DASHDOT] = wxPENSTYLE_DOT_DASH
};


SCH_LINE::SCH_LINE( const wxPoint& pos, int layer ) :
    SCH_ITEM( NULL, SCH_LINE_T )
{
    m_start = pos;
    m_end   = pos;
    m_startIsDangling = m_endIsDangling = false;
    m_size  = 0;
    m_style = -1;
    m_color = COLOR4D::UNSPECIFIED;

    switch( layer )
    {
    default:
        m_Layer = LAYER_NOTES;
        break;

    case LAYER_WIRE:
        m_Layer = LAYER_WIRE;
        break;

    case LAYER_BUS:
        m_Layer = LAYER_BUS;
        break;
    }
}


SCH_LINE::SCH_LINE( const SCH_LINE& aLine ) :
    SCH_ITEM( aLine )
{
    m_start = aLine.m_start;
    m_end = aLine.m_end;
    m_size = aLine.m_size;
    m_style = aLine.m_style;
    m_color = aLine.m_color;
    m_startIsDangling = m_endIsDangling = false;
}


EDA_ITEM* SCH_LINE::Clone() const
{
    return new SCH_LINE( *this );
}

static const char* style_names[] =
{
    "solid", "dashed", "dotted", "dash_dot", nullptr
};

const char* SCH_LINE::GetLineStyleName( int aStyle )
{
    const char * styleName = style_names[1];

    switch( aStyle )
    {
        case PLOTDASHTYPE_SOLID:
            styleName = style_names[0];
            break;

        default:
        case PLOTDASHTYPE_DASH:
            styleName = style_names[1];
            break;

        case PLOTDASHTYPE_DOT:
            styleName = style_names[2];
            break;

        case PLOTDASHTYPE_DASHDOT:
            styleName = style_names[3];
            break;
    }

    return styleName;
}


int SCH_LINE::GetLineStyleInternalId( const wxString& aStyleName )
{
    int id = -1;    // Default style id

    for( int ii = 0; style_names[ii] != nullptr; ii++ )
    {
        if( aStyleName == style_names[ii] )
        {
            id = ii;
            break;
        }
    }

    return id;
}


void SCH_LINE::Move( const wxPoint& aOffset )
{
    if( (m_Flags & STARTPOINT) == 0 && aOffset != wxPoint( 0, 0 ) )
    {
        m_start += aOffset;
        SetModified();
    }

    if( (m_Flags & ENDPOINT) == 0 && aOffset != wxPoint( 0, 0 ) )
    {
        m_end += aOffset;
        SetModified();
    }
}


#if defined(DEBUG)

void SCH_LINE::Show( int nestLevel, std::ostream& os ) const
{
    NestedSpace( nestLevel, os ) << '<' << GetClass().Lower().mb_str()
                                 << " layer=\"" << m_Layer << '"'
                                 << " startIsDangling=\"" << m_startIsDangling
                                 << '"' << " endIsDangling=\""
                                 << m_endIsDangling << '"' << ">"
                                 << " <start" << m_start << "/>"
                                 << " <end" << m_end << "/>" << "</"
                                 << GetClass().Lower().mb_str() << ">\n";
}

#endif


const EDA_RECT SCH_LINE::GetBoundingBox() const
{
    int      width = 25;

    int      xmin = std::min( m_start.x, m_end.x ) - width;
    int      ymin = std::min( m_start.y, m_end.y ) - width;

    int      xmax = std::max( m_start.x, m_end.x ) + width;
    int      ymax = std::max( m_start.y, m_end.y ) + width;

    // return a rectangle which is [pos,dim) in nature.  therefore the +1
    EDA_RECT ret( wxPoint( xmin, ymin ), wxSize( xmax - xmin + 1, ymax - ymin + 1 ) );

    return ret;
}


double SCH_LINE::GetLength() const
{
    return GetLineLength( m_start, m_end );
}


bool SCH_LINE::Save( FILE* aFile ) const
{
    bool        success = true;

    const char* layer = "Notes";
    const char* width = "Line";

    if( GetLayer() == LAYER_WIRE )
        layer = "Wire";

    if( GetLayer() == LAYER_BUS )
        layer = "Bus";

    if( fprintf( aFile, "Wire %s %s\n", layer, width ) == EOF )
    {
        success = false;
    }

    if( fprintf( aFile, "\t%-4d %-4d %-4d %-4d\n", m_start.x, m_start.y,
                 m_end.x, m_end.y ) == EOF )
    {
        success = false;
    }

    return success;
}


bool SCH_LINE::Load( LINE_READER& aLine, wxString& aErrorMsg )
{
    char  Name1[256];
    char  Name2[256];
    char* line = (char*) aLine;

    while( (*line != ' ' ) && *line )
        line++;

    if( sscanf( line, "%255s %255s", Name1, Name2 ) != 2  )
    {
        aErrorMsg.Printf( wxT( "Eeschema file segment error at line %d, aborted" ),
                          aLine.LineNumber() );
        aErrorMsg << wxT( "\n" ) << FROM_UTF8( (char*) aLine );
        return false;
    }

    m_Layer = LAYER_NOTES;

    if( Name1[0] == 'W' )
        m_Layer = LAYER_WIRE;

    if( Name1[0] == 'B' )
        m_Layer = LAYER_BUS;

    if( !aLine.ReadLine() || sscanf( (char*) aLine, "%d %d %d %d ",
                                      &m_start.x, &m_start.y, &m_end.x, &m_end.y ) != 4 )
    {
        aErrorMsg.Printf( wxT( "Eeschema file Segment struct error at line %d, aborted" ),
                          aLine.LineNumber() );
        aErrorMsg << wxT( "\n" ) << FROM_UTF8( (char*) aLine );
        return false;
    }

    return true;
}


COLOR4D SCH_LINE::GetDefaultColor() const
{
    return GetLayerColor( m_Layer );
}


void SCH_LINE::SetLineColor( const COLOR4D aColor )
{
    if( aColor == GetDefaultColor() )
        m_color = COLOR4D::UNSPECIFIED;
    else
        m_color = aColor;
}


void SCH_LINE::SetLineColor( const double r, const double g, const double b, const double a )
{
    COLOR4D newColor(r, g, b, a);

    if( newColor == GetDefaultColor() || newColor == COLOR4D::UNSPECIFIED )
        m_color = COLOR4D::UNSPECIFIED;
    else
    {
        // Eeschema does not allow alpha channel in colors
        newColor.a = 1.0;
        m_color = newColor;
    }
}


COLOR4D SCH_LINE::GetLineColor() const
{
    if( m_color == COLOR4D::UNSPECIFIED )
        return GetLayerColor( m_Layer );

    return m_color;
}

int SCH_LINE::GetDefaultStyle() const
{
    if( m_Layer == LAYER_NOTES )
        return PLOTDASHTYPE_DASH;

    return PLOTDASHTYPE_SOLID;
}


void SCH_LINE::SetLineStyle( const int aStyle )
{
    if( aStyle == GetDefaultStyle() )
        m_style = -1;
    else
        m_style = aStyle;
}


int SCH_LINE::GetLineStyle() const
{
    if( m_style >= 0 )
        return m_style;

    return GetDefaultStyle();
}


int SCH_LINE::GetDefaultWidth() const
{
    if( m_Layer == LAYER_BUS )
        return GetDefaultBusThickness();

    return GetDefaultLineThickness();
}


void SCH_LINE::SetLineWidth( const int aSize )
{
    if( aSize == GetDefaultWidth() )
        m_size = 0;
    else
        m_size = aSize;
}


int SCH_LINE::GetPenSize() const
{
    if( m_size > 0 )
        return m_size;

    if( m_Layer == LAYER_BUS )
        return GetDefaultBusThickness();

    return GetDefaultLineThickness();
}


void SCH_LINE::Draw( EDA_DRAW_PANEL* panel, wxDC* DC, const wxPoint& offset,
                     GR_DRAWMODE DrawMode, COLOR4D Color )
{
    COLOR4D color;
    int width = GetPenSize();

    if( Color != COLOR4D::UNSPECIFIED )
        color = Color;
    else if( m_color != COLOR4D::UNSPECIFIED )
        color = m_color;
    else
        color = GetLayerColor( GetState( BRIGHTENED ) ? LAYER_BRIGHTENED : m_Layer );

    GRSetDrawMode( DC, DrawMode );

    wxPoint start = m_start;
    wxPoint end = m_end;

    if( ( m_Flags & STARTPOINT ) == 0 )
        start += offset;

    if( ( m_Flags & ENDPOINT ) == 0 )
        end += offset;

    GRLine( panel->GetClipBox(), DC, start.x, start.y, end.x, end.y, width, color,
            PenStyle[ GetLineStyle() ] );

    if( m_startIsDangling )
        DrawDanglingSymbol( panel, DC, start, color );

    if( m_endIsDangling )
        DrawDanglingSymbol( panel, DC, end, color );
}


void SCH_LINE::MirrorX( int aXaxis_position )
{
    MIRROR( m_start.y, aXaxis_position );
    MIRROR( m_end.y,   aXaxis_position );
}


void SCH_LINE::MirrorY( int aYaxis_position )
{
    MIRROR( m_start.x, aYaxis_position );
    MIRROR( m_end.x,   aYaxis_position );
}


void SCH_LINE::Rotate( wxPoint aPosition )
{
    RotatePoint( &m_start, aPosition, 900 );
    RotatePoint( &m_end, aPosition, 900 );
}


/*
 * MergeOverlap try to merge 2 lines that are colinear.
 * this function expects these 2 lines have at least a common end
 */
bool SCH_LINE::MergeOverlap( SCH_LINE* aLine )
{
    wxCHECK_MSG( aLine != NULL && aLine->Type() == SCH_LINE_T, false,
                 wxT( "Cannot test line segment for overlap." ) );

    if( this == aLine || GetLayer() != aLine->GetLayer() )
        return false;

    // Search for a common end:
    if( m_start == aLine->m_start )
    {
        if( m_end == aLine->m_end )     // Trivial case
            return true;
    }
    else if( m_start == aLine->m_end )
    {
        if( m_end == aLine->m_start )     // Trivial case
            return true;
    }
    else if( m_end == aLine->m_end )
    {
        std::swap( aLine->m_start, aLine->m_end );
    }
    else if( m_end != aLine->m_start )
    {
        // No common end point, segments cannot be merged.
        return false;
    }

    bool colinear = false;

    /* Test alignment: */
    if( m_start.y == m_end.y )       // Horizontal segment
    {
        if( aLine->m_start.y == aLine->m_end.y )
        {
            colinear = true;
        }
    }
    else if( m_start.x == m_end.x )  // Vertical segment
    {
        if( aLine->m_start.x == aLine->m_end.x )
        {
            colinear = true;
        }
    }
    else
    {
        if( atan2( (double) ( m_start.x - m_end.x ), (double) ( m_start.y - m_end.y ) )
            == atan2( (double) ( aLine->m_start.x - aLine->m_end.x ),
                      (double) ( aLine->m_start.y - aLine->m_end.y ) ) )
        {
            colinear = true;
        }
    }

    // Make a segment which merge the 2 segments
    // we must find the extremums
    // i.e. the more to the left and to the right points, or
    // for horizontal segments the uppermost and the lowest point
    if( colinear )
    {
        auto less = []( const wxPoint& lhs, const wxPoint& rhs ) -> bool
        {
            if( lhs.x == rhs.x )
                return lhs.y < rhs.y;
            return lhs.x < rhs.x;
        };

        wxPoint top_left = std::min( { m_start, m_end, aLine->m_start, aLine->m_end }, less );
        wxPoint bottom_right = std::max( { m_start, m_end, aLine->m_start, aLine->m_end }, less );

        m_start = top_left;
        m_end = bottom_right;
        return true;
    }
    return false;
}


void SCH_LINE::GetEndPoints( std::vector <DANGLING_END_ITEM>& aItemList )
{
    if( GetLayer() == LAYER_NOTES )
        return;

    if( ( GetLayer() == LAYER_BUS ) || ( GetLayer() == LAYER_WIRE ) )
    {
        DANGLING_END_ITEM item( (GetLayer() == LAYER_BUS) ? BUS_START_END : WIRE_START_END, this,
                                m_start );
        aItemList.push_back( item );

        DANGLING_END_ITEM item1( (GetLayer() == LAYER_BUS) ? BUS_END_END : WIRE_END_END, this,
                                 m_end );
        aItemList.push_back( item1 );
    }
}


bool SCH_LINE::IsDanglingStateChanged( std::vector< DANGLING_END_ITEM >& aItemList )
{
    bool previousStartState = m_startIsDangling;
    bool previousEndState = m_endIsDangling;

    m_startIsDangling = m_endIsDangling = true;

    if( GetLayer() == LAYER_WIRE )
    {
        for( DANGLING_END_ITEM item : aItemList )
        {
            if( item.GetItem() == this )
                continue;

            if( item.GetType() == NO_CONNECT_END )
                continue;

            if( m_start == item.GetPosition() )
                m_startIsDangling = false;

            if( m_end == item.GetPosition() )
                m_endIsDangling = false;

            if( (m_startIsDangling == false) && (m_endIsDangling == false) )
                break;
        }
    }
    else if( GetLayer() == LAYER_BUS || GetLayer() == LAYER_NOTES )
    {
        // Lines on the notes layer and the bus layer cannot be tested for dangling ends.
        previousStartState = previousEndState = m_startIsDangling = m_endIsDangling = false;
    }

    return ( previousStartState != m_startIsDangling ) || ( previousEndState != m_endIsDangling );
}


bool SCH_LINE::IsSelectStateChanged( const wxRect& aRect )
{
    bool previousState = IsSelected();

    if( aRect.Contains( m_start ) && aRect.Contains( m_end ) )
    {
        SetFlags( SELECTED );
        ClearFlags( STARTPOINT | ENDPOINT );
    }
    else if( aRect.Contains( m_start ) )
    {
        ClearFlags( STARTPOINT );
        SetFlags( SELECTED | ENDPOINT );
    }
    else if( aRect.Contains( m_end ) )
    {
        ClearFlags( ENDPOINT );
        SetFlags( SELECTED | STARTPOINT );
    }
    else
    {
        ClearFlags( SELECTED | STARTPOINT | ENDPOINT );
    }

    return previousState != IsSelected();
}


bool SCH_LINE::IsConnectable() const
{
    if( m_Layer == LAYER_WIRE || m_Layer == LAYER_BUS )
        return true;

    return false;
}


void SCH_LINE::GetConnectionPoints( std::vector< wxPoint >& aPoints ) const
{
    aPoints.push_back( m_start );
    aPoints.push_back( m_end );
}


wxString SCH_LINE::GetSelectMenuText() const
{
    wxString menuText, txtfmt, orient;

    if( m_start.x == m_end.x )
        orient = _("Vert.");
    else if( m_start.y == m_end.y )
        orient = _("Horiz.");

    switch( m_Layer )
    {
    case LAYER_NOTES:
        txtfmt = _( "%s Graphic Line from (%s,%s) to (%s,%s)" );
        break;

    case LAYER_WIRE:
        txtfmt = _( "%s Wire from (%s,%s) to (%s,%s)" );
        break;

    case LAYER_BUS:
        txtfmt = _( "%s Bus from (%s,%s) to (%s,%s)" );
        break;

    default:
        txtfmt += _( "%s Line on Unknown Layer from (%s,%s) to (%s,%s)" );
    }

    menuText.Printf( txtfmt, GetChars( orient ),
                     GetChars( CoordinateToString( m_start.x ) ),
                     GetChars( CoordinateToString( m_start.y ) ),
                     GetChars( CoordinateToString( m_end.x ) ),
                     GetChars( CoordinateToString( m_end.y ) ) );

    return menuText;
}


BITMAP_DEF SCH_LINE::GetMenuImage() const
{
    if( m_Layer == LAYER_NOTES )
        return add_dashed_line_xpm;
    else if( m_Layer == LAYER_WIRE )
        return add_line_xpm;

    return add_bus_xpm;
}


void SCH_LINE::GetNetListItem( NETLIST_OBJECT_LIST& aNetListItems,
                               SCH_SHEET_PATH*      aSheetPath )
{
    // Net list item not required for graphic lines.
    if( (GetLayer() != LAYER_BUS) && (GetLayer() != LAYER_WIRE) )
        return;

    NETLIST_OBJECT* item = new NETLIST_OBJECT();
    item->m_SheetPath = *aSheetPath;
    item->m_SheetPathInclude = *aSheetPath;
    item->m_Comp = (SCH_ITEM*) this;
    item->m_Start = m_start;
    item->m_End = m_end;

    if( GetLayer() == LAYER_BUS )
    {
        item->m_Type = NET_BUS;
    }
    else            /* WIRE */
    {
        item->m_Type = NET_SEGMENT;
    }

    aNetListItems.push_back( item );
}


bool SCH_LINE::operator <( const SCH_ITEM& aItem ) const
{
    if( Type() != aItem.Type() )
        return Type() < aItem.Type();

    SCH_LINE* line = (SCH_LINE*) &aItem;

    if( GetLength() != line->GetLength() )
        return GetLength() < line->GetLength();

    if( m_start.x != line->m_start.x )
        return m_start.x < line->m_start.x;

    if( m_start.y != line->m_start.y )
        return m_start.y < line->m_start.y;

    return false;
}


bool SCH_LINE::HitTest( const wxPoint& aPosition, int aAccuracy ) const
{
    return TestSegmentHit( aPosition, m_start, m_end, aAccuracy );
}


bool SCH_LINE::HitTest( const EDA_RECT& aRect, bool aContained, int aAccuracy ) const
{
    if( m_Flags & ( STRUCT_DELETED | SKIP_STRUCT ) )
        return false;

    EDA_RECT rect = aRect;

    if ( aAccuracy )
        rect.Inflate( aAccuracy );

    if( aContained )
        return rect.Contains( m_start ) && rect.Contains( m_end );

    return rect.Intersects( m_start, m_end );
}

void SCH_LINE::SwapData( SCH_ITEM* aItem )
{
    SCH_LINE* item = (SCH_LINE*) aItem;

    std::swap( m_Layer, item->m_Layer );

    std::swap( m_start, item->m_start );
    std::swap( m_end, item->m_end );
    std::swap( m_startIsDangling, item->m_startIsDangling );
    std::swap( m_endIsDangling, item->m_endIsDangling );
    std::swap( m_style, item->m_style );
    std::swap( m_size, item->m_size );
    std::swap( m_color, item->m_color );
}

bool SCH_LINE::doIsConnected( const wxPoint& aPosition ) const
{
    if( m_Layer != LAYER_WIRE && m_Layer != LAYER_BUS )
        return false;

    return IsEndPoint( aPosition );
}


void SCH_LINE::Plot( PLOTTER* aPlotter )
{
    if( m_color != COLOR4D::UNSPECIFIED )
        aPlotter->SetColor( m_color );
    else
        aPlotter->SetColor( GetLayerColor( GetLayer() ) );

    aPlotter->SetCurrentLineWidth( GetPenSize() );

    aPlotter->SetDash( GetLineStyle() );

    aPlotter->MoveTo( m_start );
    aPlotter->FinishTo( m_end );

    aPlotter->SetDash( 0 );
}


void SCH_LINE::SetPosition( const wxPoint& aPosition )
{
    m_end = m_end - ( m_start - aPosition );
    m_start = aPosition;
}


wxPoint SCH_LINE::MidPoint()
{
    return wxPoint( ( m_start.x + m_end.x ) / 2, ( m_start.y + m_end.y ) / 2 );
}


int SCH_EDIT_FRAME::EditLine( SCH_LINE* aLine, bool aRedraw )
{
    if( aLine == NULL )
        return wxID_CANCEL;

    // We purposely disallow editing everything except graphic lines
    if( aLine->GetLayer() != LAYER_NOTES )
        return wxID_CANCEL;

    DIALOG_EDIT_LINE_STYLE dlg( this );
    wxString units = GetAbbreviatedUnitsLabel( g_UserUnit );
    int old_style = aLine->GetLineStyle();
    int old_width = aLine->GetPenSize();
    COLOR4D old_color = aLine->GetLineColor();

    dlg.SetDefaultStyle( aLine->GetDefaultStyle() );
    dlg.SetDefaultWidth( StringFromValue( g_UserUnit, aLine->GetDefaultWidth(), false ) );
    dlg.SetDefaultColor( aLine->GetDefaultColor() );

    dlg.SetWidth( StringFromValue( g_UserUnit, old_width, false ) );
    dlg.SetStyle( old_style );
    dlg.SetLineWidthUnits( units );
    dlg.SetColor( old_color, true );

    dlg.Layout();
    dlg.Fit();
    dlg.SetMinSize( dlg.GetSize() );
    if( dlg.ShowModal() == wxID_CANCEL )
        return wxID_CANCEL;

    int new_width = std::max( 1, ValueFromString( dlg.GetWidth() ) );
    int new_style = dlg.GetStyle();
    COLOR4D new_color = dlg.GetColor();

    if( new_width != old_width || new_style != old_style || new_color != old_color )
    {
        SaveCopyInUndoList( (SCH_ITEM*) aLine, UR_CHANGED );
        aLine->SetLineWidth( new_width );
        aLine->SetLineStyle( new_style );
        aLine->SetLineColor( new_color );

        OnModify();
        if( aRedraw )
            m_canvas->Refresh();
    }

    return wxID_OK;
}
