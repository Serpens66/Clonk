/*-- Neues Objekt --*/

#strict

local seg1,seg2;

func Initialize() {
  SetAction("Closed");
  SetPosition(GetX(), GetY()+6);
  DigFreeRect(GetX()-66, GetY()-26, 132, 18);
//seg1=CreateObject(BAS1, -53, 0);
//seg2=CreateObject(BAS1, 53, 0);
  return(1);
}

func BeginMaskr()	{
	SetSolidMask(0,0,132,8);
}

func EndMaskr()	{
	SetSolidMask(0,0,0,0);
}

private func UpdateSolidMaskr()
{
  if (GetAction() eq "Open")
    SetSolidMask(GetPhase() * 132, 0, 132, 8, 0, 0);
  if (GetAction() eq "Close")
    SetSolidMask(GetDefCoreVal("Width", "DefCore", GetID()) - GetPhase() * 132, 0, 132, 8, 0, 0);
}  

func Timerr()	{
	if(GetAction() eq "Opened")
		{
		EndMaskr();
		}
	if(GetAction() eq "Closed")
		{
		BeginMaskr();
		}

	if(FindObject(BELC, -40, -45, 80, 85) || FindObject(SLP1, -40, -45, 80, 85) || FindObject(CHLP, -40, -45, 80, 85))
		{
		if(GetAction() eq "Closed")
			{
			SetAction("Open");
			Sound("SteelGate2");
			}
		}
	else
		{
		if(GetAction() eq "Opened")
			{
			SetAction("Close");
			Sound("SteelGate2");
			}
		}
}

func Destruction()
{
if(seg1)RemoveObject(seg1);
if(seg2)RemoveObject(seg2);
_inherited();
}
