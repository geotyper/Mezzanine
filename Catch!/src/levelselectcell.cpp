#ifndef _levelselectcell_cpp
#define _levelselectcell_cpp

#include "levelselectcell.h"

LevelSelectCell::LevelSelectCell(const String& name, const UI::RenderableRect& Rect, UI::Layer* parent)
    : UI::Cell(name,parent),
      PreviewImageOffset(Vector2(-0.005,-0.005)),
      PreviewBorderOffset(Vector2(-0.01,-0.01)),
      LevelTitleOffset(Vector2(0.115,0.01)),
      EarnedMaxScoreOffset(Vector2(0.18,0.05))
{
    const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
    std::pair<Whole,Real> Result;
    if(Rect.Relative)
    {
        RelPosition = Rect.Position;
        RelSize = Rect.Size;
        Result = World::GetWorldPointer()->GetUIManager()->SuggestGlyphIndex((Rect.Size.Y * 0.36) * WinDim.Y,Parent->GetParent()->GetPrimaryAtlas());

        CellBack = new UI::Rectangle(Rect,Parent);
        PreviewImage = new UI::Rectangle(UI::RenderableRect(Rect.Position+PreviewImageOffset,Vector2(Rect.Size.Y * 1.05,Rect.Size.Y * 1.05),true),Parent);
        PreviewBorder = new UI::Rectangle(UI::RenderableRect(Rect.Position+PreviewBorderOffset,Vector2(Rect.Size.Y * 1.15,Rect.Size.Y * 1.15),true),Parent);
        LevelTitle = new UI::Caption(Name+"LT",UI::RenderableRect(Rect.Position+LevelTitleOffset,Vector2(Rect.Size.X * 0.6,Rect.Size.Y * 0.36),true),Result.first,Name,Parent);
        EarnedMaxScore = new UI::Caption(Name+"EMS",UI::RenderableRect(Rect.Position+EarnedMaxScoreOffset,Vector2(Rect.Size.X * 0.4,Rect.Size.Y * 0.36),true),Result.first,"0/0",Parent);
    }else{
        RelPosition = Rect.Position / WinDim;
        RelSize = Rect.Size / WinDim;
        Result = World::GetWorldPointer()->GetUIManager()->SuggestGlyphIndex(Rect.Size.Y * 0.36,Parent->GetParent()->GetPrimaryAtlas());
    }

    if(1.f != Result.second)
    {
        //LevelTitle->SetTextScale(Result.second);
        //EarnedMaxScore->SetTextScale(Result.second);
    }
}

LevelSelectCell::~LevelSelectCell()
{
    delete CellBack;
    delete PreviewImage;
    delete PreviewBorder;
    delete LevelTitle;
    delete EarnedMaxScore;
}

void LevelSelectCell::Update(bool Force)
{

}

void LevelSelectCell::SetVisible(bool visible)
{
    CellBack->SetVisible(visible);
    PreviewImage->SetVisible(visible);
    PreviewBorder->SetVisible(visible);
    LevelTitle->SetVisible(visible);
    EarnedMaxScore->SetVisible(visible);
    Visible = visible;
}

bool LevelSelectCell::IsVisible()
{
    return Visible;
}

void LevelSelectCell::Show()
{
    CellBack->Show();
    PreviewImage->Show();
    PreviewBorder->Show();
    LevelTitle->Show();
    EarnedMaxScore->Show();
    Visible = true;
}

void LevelSelectCell::Hide()
{
    CellBack->Hide();
    PreviewImage->Hide();
    PreviewBorder->Hide();
    LevelTitle->Hide();
    EarnedMaxScore->Hide();
    Visible = false;
}

bool LevelSelectCell::CheckMouseHover()
{
    if(!Visible)
        return false;
    if(CellBack->CheckMouseHover()) return true;
    else if(PreviewImage->CheckMouseHover()) return true;
    else if(PreviewBorder->CheckMouseHover()) return true;
    else return false;
}

void LevelSelectCell::SetPosition(const Vector2& Position)
{
    RelPosition = Position;
    CellBack->SetPosition(Position);
    PreviewImage->SetPosition(Position + PreviewImageOffset);
    PreviewBorder->SetPosition(Position + PreviewBorderOffset);
    LevelTitle->SetPosition(Position + LevelTitleOffset);
    EarnedMaxScore->SetPosition(Position + EarnedMaxScoreOffset);
}

Vector2 LevelSelectCell::GetPosition()
{
    return RelPosition;
}

void LevelSelectCell::SetActualPosition(const Vector2& Position)
{
    SetPosition(Position / Parent->GetParent()->GetViewportDimensions());
}

Vector2 LevelSelectCell::GetActualPosition()
{
    return RelPosition * Parent->GetParent()->GetViewportDimensions();
}

void LevelSelectCell::SetSize(const Vector2& Size)
{
    RelSize = Size;
    CellBack->SetSize(Size);
    PreviewImage->SetSize(Vector2(Size.Y * 1.05,Size.Y * 1.05));
    PreviewBorder->SetSize(Vector2(Size.Y * 1.15,Size.Y * 1.15));
    LevelTitle->SetSize(Vector2(Size.X * 0.6,Size.Y * 0.36));
    EarnedMaxScore->SetSize(Vector2(Size.X * 0.4,Size.Y * 0.36));

    SetPosition(RelPosition);
}

Vector2 LevelSelectCell::GetSize()
{
    return RelSize;
}

void LevelSelectCell::SetActualSize(const Vector2& Size)
{
    const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
    RelSize = Size / WinDim;
    CellBack->SetActualSize(Size);
    PreviewImage->SetActualSize(Vector2(Size.Y * 1.05,Size.Y * 1.05));
    PreviewBorder->SetActualSize(Vector2(Size.Y * 1.15,Size.Y * 1.15));
    LevelTitle->SetActualSize(Vector2(Size.X * 0.6,Size.Y * 0.36));
    EarnedMaxScore->SetActualSize(Vector2(Size.X * 0.4,Size.Y * 0.36));
    //SetSize(Size / WinDim);
    SetPosition(RelPosition);
}

Vector2 LevelSelectCell::GetActualSize()
{
    return RelSize * Parent->GetParent()->GetViewportDimensions();
}

void LevelSelectCell::DoSelectedItems()
{
    /// @todo This or the cell grid class really needs to actually show what is selected somehow.
}

String LevelSelectCell::GetLevelName()
{
    return LevelTitle->GetText();
}

UI::Rectangle* LevelSelectCell::GetCellBack()
{
    return CellBack;
}

UI::Rectangle* LevelSelectCell::GetPreviewImage()
{
    return PreviewImage;
}

UI::Rectangle* LevelSelectCell::GetPreviewBorder()
{
    return PreviewBorder;
}

UI::Caption* LevelSelectCell::GetLevelTitle()
{
    return LevelTitle;
}

UI::Caption* LevelSelectCell::GetEarnedMaxScore()
{
    return EarnedMaxScore;
}

LevelSelectCB::LevelSelectCB(LevelSelectCell* CallerCell)
    : UI::CellCallback(CallerCell),
      LevelCell(CallerCell)
{
}

LevelSelectCB::~LevelSelectCB()
{
}

void LevelSelectCB::DoSelectedItems()
{
    LevelCell->GetCellBack()->SetBackgroundSprite("MMLevelCellBackSelected");
}

void LevelSelectCB::DoUnselectedItems()
{
    LevelCell->GetCellBack()->SetBackgroundSprite("MMLevelCellBack");
}

#endif
