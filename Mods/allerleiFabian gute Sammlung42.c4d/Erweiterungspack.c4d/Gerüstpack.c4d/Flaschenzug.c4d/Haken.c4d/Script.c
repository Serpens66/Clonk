/*-- Haken --*/

#strict

local Flaschenzug,Drehrad;
local RangeTop,TargetPos;

protected func Initialize()
{
  SetAction("Wait");
  RangeTop= GetY();
}

public func Init(pBuilding,pWheel)
{
  Flaschenzug= pBuilding;
  Drehrad= pWheel;
}

private func TimerCall()
{
  if(!Flaschenzug) return(0,SetAction("Idle"));
}

public func DoControlUp(pClonk)
{
  if(!Flaschenzug) return(0,SetAction("Idle"));
  if(!KeyIsOccupied()) return(0);
  Sound("Click");
  if(Hostile(GetController(pClonk),GetOwner())) return(0);
  SetComDir(COMD_Up());
  SetAction("FloatUp");  
}

public func DoControlDown(pClonk)
{
  if(!Flaschenzug) return(0,SetAction("Idle"));
  if(!KeyIsOccupied()) return(0);
  Sound("Click");
  if(Hostile(GetController(pClonk),GetOwner())) return(0);
  SetComDir(COMD_Down());
  SetAction("Float");
}

public func DoControlDig(pClonk)
{
  Sound("Click");
  Stop();
}

public func Stop()
{
  if(!Flaschenzug) return(0,SetAction("Idle"));
  ObjectSetAction(Drehrad,"Normal");
  SetAction("Wait");
  SetComDir(COMD_Stop());
  SetYDir();
}

private func RangeTopCheck()
{
  if(GetY()<= RangeTop) return(Stop());
}

private func ContactBottom()
{
  return(Stop());
}

private func Waiting()
{
}

private func FindContainer()
{
  var obj;
  while(obj=FindObject(0,-20,-20,40,40,0,0,0,NoContainer(),obj))
   if(ObjectCall(obj,"IsContainer"))
    return(obj);
}

private func FindAttachedContainer()
{
  var obj;
  while(obj=FindObject(0,-20,-20,40,40,0,0,this(),NoContainer(),obj))
   if(ObjectCall(obj,"IsContainer"))
    return(obj);
}

protected func Destruction()
{
  var obj;
  if(obj= FindObject(GHLP,0,0,0,0,0,"Attach",this()))
    {
    var line;
    if(line= FindObject(PWRL,0,0,0,0,0,"Connect",obj)) RemoveObject(line);
    RemoveObject(obj);
    }
}

public func KeyIsOccupied() { return(ObjectCall(Drehrad,"IsOccupied")); }

public func GetResearchBase() { return(GRS3); }

/*-- Tooltime :) --*/

// int- X-Koordinate für CreateObject
public func GetXPos(TurnedLeft)
{
  if(TurnedLeft) return(-30);
  return(+32);
}
// int- Y-Koordinate für CreateObject
public func GetYPos() { return(-10); }

// bool - Gibt an, ob "Stop" aufgerufen werden soll, wenn im Drehrad das Menü geöffnet wird
public func CallStop_Menu() { return(1); }

// bool - Bedingung, damit im Drehrad ein Menüeintrag hinzugefügt wird
public func Condition_MenuItem() { return(FindContainer() || FindAttachedContainer()); }

// string - Text für den Menüeintrag (von dem oben die Rede war)
public func GetMenuCaptain()
{
  if(FindAttachedContainer()) return("$Unhook$");
  return("$Hook$");
}

// id - ID für den Menüeintrag (von dem oben die Rede war)
public func GetMenuItem() { return(GetID(FindContainer())); }

// Wird vom Menüeintrag (von dem oben die Rege war) aufgerufen
public func UseTool(pObject)
{
  Sound("Connect");
  if(FindAttachedContainer())
    {
    ObjectSetAction(FindAttachedContainer(),"Idle");
    SetVertexXY(2,0,4);
    }
  else
    {
    var container= FindContainer();
    if(pObject) container= pObject;
    ObjectSetAction(container,"Attach",this());
    SetVertexXY(2,0,container->~ GetToolVertexY());
    while(Stuck()) SetPosition(GetX(),GetY()-1);
    }
}

// Tool-Erkennung.
public func IsTooool() { return(1); }