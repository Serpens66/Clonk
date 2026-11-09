/*-- Flagge abmontieren und Uplink Verwendung --*/

// ein appendto an alle Basengebäude, damit man per contextmenü die Flagge abmontieren kann. Ist besser, als wenn man dafür die Regel Flagge abnehmbar einschalten muss und die Flagge einfach nur nervt, weil sie ständig ausversehen mitgenommen wird.


#strict 3


#appendto CST1
#appendto CST2
#appendto CST3
#appendto HUT1
#appendto HUT2
#appendto HUT3
#appendto IGLO
#appendto CNBS
#appendto CBBS
#appendto SDWU
#appendto SMBS
#appendto CPBX
#appendto TN5B
#appendto TENT
#appendto TR33  // baumversteck
#appendto TABA // kleine basis
#appendto TAGB // große basis
#appendto TAMB // mittlere basis

func IsBaseBuilding() // alle potentielle Basen
{
    return(1);
}

//Überprüfung ob Flagge vorhanden
private func HasFlagge() {
	return FindObject2(Find_Or(Find_ID(FLAG),Find_ID(_FLG)), Find_Action("FlyBase"), Find_ActionTarget(this),Find_Not(Find_Func("IstPseudoflagge")));
}

// wenn Flagge da, dann option des abomntierens
protected func ContextRemoveFlagge() {
	[Flagge abmontieren|Image=FLAG|Condition=HasFlagge]
	HasFlagge() -> SetAction("Idle");
}

// um Sensoren via Kontextmenü abnehmen zu können, wenn man so nicht mehr dran kommt, nur in Basen.

func HatLebeSensorenplatziert()
{
var upl1 = FindObject2(Find_ID(UPLT),Find_Action("Turn"),Find_Distance(50));
var upl2 = FindObject2(Find_ID(UPLB),Find_Action("Turn"),Find_Distance(50));
var flag = FindObject2(Find_ID(FLAG),Find_Action("FlyBase"),Find_Distance(50));
var obj = GetActionTarget(nil,upl1);
if(!obj) obj = GetActionTarget(nil,upl2);
if(!obj) obj = GetActionTarget(nil,flag);
for (var sensor in FindObjects(Find_ID(LS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
if(And(sensor, obj)) return(1);
}
func ContextNehmeLebeSensorenAb(pCaller)
{
	[Lebewesensensoren abmontieren|Image=LS4K|Condition=HatLebeSensorenplatziert]
        for (var sensor in FindObjects(Find_ID(LS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
sensor->~detach2();
}



func HatWasserSensorenplatziert()
{
var upl1 = FindObject2(Find_ID(UPLT),Find_Action("Turn"),Find_Distance(50));
var upl2 = FindObject2(Find_ID(UPLB),Find_Action("Turn"),Find_Distance(50));
var flag = FindObject2(Find_ID(FLAG),Find_Action("FlyBase"),Find_Distance(50));
var obj = GetActionTarget(nil,upl1);
if(!obj) obj = GetActionTarget(nil,upl2);
if(!obj) obj = GetActionTarget(nil,flag);
for (var sensor in FindObjects(Find_ID(WS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
if(And(sensor, obj)) return(1);
}
func ContextNehmeWasserSensorenAb(pCaller)
{
	[Wassersensoren abmontieren|Image=WS4K|Condition=HatWasserSensorenplatziert]
        for (var sensor in FindObjects(Find_ID(WS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
sensor->~detach2();
}



func HatSchalterplatziert()
{
var upl1 = FindObject2(Find_ID(UPLT),Find_Action("Turn"),Find_Distance(50));
var upl2 = FindObject2(Find_ID(UPLB),Find_Action("Turn"),Find_Distance(50));
var flag = FindObject2(Find_ID(FLAG),Find_Action("FlyBase"),Find_Distance(50));
var obj = GetActionTarget(nil,upl1);
if(!obj) obj = GetActionTarget(nil,upl2);
if(!obj) obj = GetActionTarget(nil,flag);
for (var sensor in FindObjects(Find_ID(SS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
if(And(sensor, obj)) return(1);
}
func ContextNehmeSchalterAb(pCaller)
{
	[Schalter abmontieren|Image=SS4K|Condition=HatSchalterplatziert]
        for (var sensor in FindObjects(Find_ID(SS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
sensor->~detach2();
}



func HatKnopfplatziert()
{
var upl1 = FindObject2(Find_ID(UPLT),Find_Action("Turn"),Find_Distance(50));
var upl2 = FindObject2(Find_ID(UPLB),Find_Action("Turn"),Find_Distance(50));
var flag = FindObject2(Find_ID(FLAG),Find_Action("FlyBase"),Find_Distance(50));
var obj = GetActionTarget(nil,upl1);
if(!obj) obj = GetActionTarget(nil,upl2);
if(!obj) obj = GetActionTarget(nil,flag);
for (var sensor in FindObjects(Find_ID(BS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
if(And(sensor, obj)) return(1);
}
func ContextNehmeKnopfAb(pCaller)
{
	[Knöpfe abmontieren|Image=BS4K|Condition=HatKnopfplatziert]
        for (var sensor in FindObjects(Find_ID(BS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
sensor->~detach2();
}


func HatPINplatziert()
{
var upl1 = FindObject2(Find_ID(UPLT),Find_Action("Turn"),Find_Distance(50));
var upl2 = FindObject2(Find_ID(UPLB),Find_Action("Turn"),Find_Distance(50));
var flag = FindObject2(Find_ID(FLAG),Find_Action("FlyBase"),Find_Distance(50));
var obj = GetActionTarget(nil,upl1);
if(!obj) obj = GetActionTarget(nil,upl2);
if(!obj) obj = GetActionTarget(nil,flag);
for (var sensor in FindObjects(Find_ID(PS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
if(And(sensor, obj)) return(1);
}
func ContextNehmePINAb(pCaller)
{
	[PIN Geräte abmontieren|Image=PS4K|Condition=HatPINplatziert]
        for (var sensor in FindObjects(Find_ID(PS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
sensor->~detach2();
}



func HatKartenleserplatziert()
{
var upl1 = FindObject2(Find_ID(UPLT),Find_Action("Turn"),Find_Distance(50));
var upl2 = FindObject2(Find_ID(UPLB),Find_Action("Turn"),Find_Distance(50));
var flag = FindObject2(Find_ID(FLAG),Find_Action("FlyBase"),Find_Distance(50));
var obj = GetActionTarget(nil,upl1);
if(!obj) obj = GetActionTarget(nil,upl2);
if(!obj) obj = GetActionTarget(nil,flag);
for (var sensor in FindObjects(Find_ID(CS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
if(And(sensor, obj)) return(1);
}
func ContextNehmeKartenleserAb(pCaller)
{
	[Kartenleser abmontieren|Image=CS4K|Condition=HatKartenleserplatziert]
        for (var sensor in FindObjects(Find_ID(CS4K), Find_Owner(GetOwner()),Find_Or(Find_Action("Attach"),Find_Action("Wall"))))
sensor->~detach2();
}



// Uplink-Steuerung

protected func ContainedUp(a,b,c)
{
  [Kauf|Image=UPLI|Condition=GetUplink]
  var uplink = GetUplink();
  if (!uplink)  return(_inherited(a,b,c));
  if (HasEMP()) return(Sound("Error"));
  uplink->KeyUp(Par(nil));
  return(1);
  
}

protected func ContainedDig(a,b,c)
{
  [Verkauf|Image=UPLI|Condition=GetUplink]
  var uplink = GetUplink();
  if (!uplink)  return(_inherited(a,b,c));
  if (HasEMP()) return(Sound("Error"));
  uplink->KeyDig(Par(nil));
  return(1);
}

private func GetUplink() 
{ 
  var uplink = FindObject(UPLB,nil,nil,nil,nil,nil,"Turn",this());
  if (!uplink)
      uplink = FindObject(UPLT,nil,nil,nil,nil,nil,"Turn",this());
  return(uplink);
}




