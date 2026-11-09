/*-- Sensor-Functions --*/

#strict 3

//Variablen
local targets;
local action_ids;
local action_trigger_masks;
local state_desc;

local last_targets;
local state_selection;
local state;

local pUser;


func IsRepa(){return(1);}
func IsBulletExtraTarget(){return(1);}

public func Damage(a,b,c,d,e,f,g)
{
var hp;
var schaden;
hp = GetDefCoreVal("BlastIncinerate","DefCore");
schaden = GetDamage();
if ((schaden ?? 0) >= (hp ?? 0)) {Incinerate();};
_inherited(a,b,c,d,e,f,g);
}



func Initialize()
{
  targets=CreateArray();
  action_ids=CreateArray();
  action_trigger_masks=CreateArray();
  last_targets=CreateArray();
  state_desc = CreateArray();
  state_selection = nil;
  state = nil;
  SetAction("Idle");
  return(1);
}

//Die Menüs
public func Activate(pCaller)
{
    if(Hostile (GetOwner(pCaller),GetOwner()))
      return(Sound ("CommandFailure1",nil,pCaller,100,GetOwner(pCaller),nil));

    pUser = pCaller;
    Setup();
}

public func ControlDigDouble(pCaller)
{
    Activate(pCaller);
} 

func Setup()
{
    CreateMenu (GetID(),pUser,nil,nil,nil,nil,1,1);
    AddMenuItem ("$ADD_TARGET$","addtarget",GetID(),pUser,nil,nil,"$ADD_TARGET_DESC$");
    AddMenuItem ("$DEL_TARGET$","deltarget",GetID(),pUser,nil,nil,"$DEL_TARGET_DESC$");

    if(Or((!!SEqual(GetAction(), "Wall")), (!!SEqual(GetAction(), "Attach"))))
      AddMenuItem ("$DETACH$","detach",GetID(),pUser,nil,nil,"$DETACH_DESC$");
    else
    {
      if(And((!GBackLiquid()), (!Equal(GetMaterial(), Material ("Sky")))))
        AddMenuItem ("$ATTACH_WALL$","attach2wall",GetID(),pUser,nil,nil,"$ATTACH_WALL_DESC$");



      if(And((!GBackLiquid()), (Equal(GetMaterial(), Material ("Sky")))))
       AddMenuItem ("$ATTACH_SKY$","attach2sky",GetID(),pUser,nil,nil,"$ATTACH_SKY_DESC$");


// deaktiviert, weil das oft für Probleme sorgt
//      var objects = FindObjects(Find_AtPoint(), Find_Category(C4D_Vehicle | C4D_Structure));
//      if (GetLength(objects))
//      AddMenuItem ("$ATTACH_OBJECT$","Attach2Object",GetID(),pUser,0,0,"$ATTACH_OBJECT_DESC$");
    }
    ExtraSetup(nil,pUser);
}

func deltarget()
{
    CreateMenu (GetID(),pUser,nil,nil,nil,nil,1,nil);
    var i = nil;
    for(var obj in targets)
    {
      AddMenuItem (GetName(obj),"deltarget2",GetID(obj),pUser,nil,i,"$DEL_TARGET_DESC$");
      S42MigrationStep(i, 1, true);
    }
}

func deltarget2(idItem,iIndex)
{
    DeleteArrayItem(iIndex,targets);
    DeleteArrayItem(iIndex,action_ids);
    DeleteArrayItem(iIndex,action_trigger_masks);
    deltarget();
}

func addtarget()
{
    last_targets = FindObjects (Find_Or (Find_Func ("GetTriggerName"),Find_Func ("ConsoleControl")),Find_Distance(40));
    if(!last_targets)
        return nil;
    CreateMenu (GetID(),pUser,nil,nil,nil,nil,1,nil);
    for(var i = nil;(i ?? 0) <
(GetLength(last_targets) ?? 0);
S42MigrationStep(i, 1, true))
    {
      if (!Hostile(last_targets[(i ?? 0)]->GetOwner(), GetOwner()))
        AddMenuItem (GetName(last_targets[(i ?? 0)]),Format("SetTriggerAction(%d)",i),GetID(last_targets[(i ?? 0)]),pUser,nil,nil);
    }
}

func FindAttachTarget()
{
  var obj=nil;
  CreateMenu(GetID(),Par(1),nil,nil,"$ATTACH$",nil,1,nil);
  while(obj=FindObject(nil,-15,-80,30,90,nil,nil,nil,NoContainer(),obj))
    AddMenuItem(Format("%s",GetName(obj)),"Toolit",GetID(obj),Par(1),nil,obj,nil,nil,nil,nil);
}

func Attach(pTarget)
{
  SetAction ("Attach",pTarget);
  return(1);
}

func SetTriggerAction(a)
{
    CreateMenu (GetID(),pUser,nil,nil,nil,nil,1,nil);
    var i = 1;
    while(Equal(GetType (TargetTriggerName(last_targets[(a ?? 0)],i)), C4V_String))
    {
        AddMenuItem (TargetTriggerName(last_targets[(a ?? 0)],i),Format("SetTriggerState(%d,%d,0)",a,i),RSR1,pUser,nil,nil);
        S42MigrationStep(i, 1, true);
    }
}

func TargetTriggerName(pTarget,iTrigger)
{
  var string;
  string = pTarget->~GetTriggerName(iTrigger);

  if(!string)
  {
    string = pTarget->~ConsoleControl(iTrigger);
  }

  return(string);
}

func SetTriggerState(a,b,c)
{
    var active = nil;
    var selection = c;
    var state_pos = nil;
    CreateMenu (GetID(),pUser,nil,nil,nil,nil,1,nil);
    for(var i = nil;(i ?? 0)<(GetLength(state_desc) ?? 0);S42MigrationStep(i, 1, true))
    {
	state_pos = i;
        active = GetState(state_selection,i);
	AddMenuItem (state_desc[(i ?? 0)],Format("SwitchState(%d,%d,%d)",a,b,i),SENS,pUser,nil,nil,"",2,active);
	active = nil;
    }
    AddMenuItem ("$FINISHED$",Format("AddTarget2(%d,%d,%d)",a,b,state_selection),GetID(),pUser,nil,nil);
    SelectMenuItem (selection,pUser);
}

func SwitchState(a,b,c)
{
	if(Equal(GetState(state_selection,c), nil))
		SetState(state_selection,c,1);
	else
	{
		if(Equal(GetState(state_selection,c), 1))
			SetState(state_selection,c,2);
		else
		{
			if(Equal(GetState(state_selection,c), 2))
				SetState(state_selection,c,nil);
		}
	}
	SetTriggerState(a,b,c);
}

func ExtraSetup(){return(nil);}
func ExtraMenu(){return(nil);}
func GetAdditionalInfo(){return(nil);}

//Bit-Funktionen
func GetBit(&bitmap,bitpos)
{
    var bit = 1 << (bitpos ?? 0);
    if((bitmap ?? 0) & (bit ?? 0))
        return true;
    return nil;
}

func SetBit(&bitmap,bitpos,value)
{
    var bit = 1 << (bitpos ?? 0);
    if(Equal(value, 1))
        bitmap = (bitmap ?? 0) | (bit ?? 0);
    else
        bitmap = (bitmap ?? 0) & ~ (bit ?? 0);
}

func SwitchBit(&bitmap,bitpos)
{
    var bit = 1 << (bitpos ?? 0);
    bitmap = (bitmap ?? 0) ^ (bit ?? 0);
}

//Trigger-Mask
func AddState(desc)
{
    AddToArray(state_desc,desc);
}

func SetState(&bitmap,action_id,value)
{
	if(Equal(value, nil))
	{
		SetBit(bitmap,(action_id ?? 0)*2,nil);
		SetBit(bitmap,(action_id ?? 0)*2+1,nil);
		return(nil);
	}
	if(Equal(value, 1))
	{
		SetBit(bitmap,(action_id ?? 0)*2,1);
		SetBit(bitmap,(action_id ?? 0)*2+1,nil);
		return(1);
	}
	else
	{
		SetBit(bitmap,(action_id ?? 0)*2,nil);
		SetBit(bitmap,(action_id ?? 0)*2+1,1);
		return(2);
	}
}

func GetState(&bitmap,action_id)
{
	if(Equal(GetBit(bitmap,(action_id ?? 0)*2), 1))
		return(1);
	if(Equal(GetBit(bitmap,(action_id ?? 0)*2+1), 1))
		return(2);
	return(nil);
}

//Target-Array
func AddTarget(target,trigger,mask)
{
    AddToArray(targets,target);
    AddToArray(action_ids,trigger);
    AddToArray(action_trigger_masks,mask);
}

func AddTarget2(target,trigger,mask)
{
    AddTarget(last_targets[(target ?? 0)],trigger,mask);
    state_selection = nil;
    ExtraMenu();
}

private func AddToArray(&a, add)
{
  a[(GetLength(a) ?? 0)] = add;
}

func ResetArray(&a)
{
    SetLength(a,nil);
    a = CreateArray();
}

//Triggering
func TriggerActions()
{
    for(var i = nil;(i ?? 0) < (GetLength(targets) ?? 0);S42MigrationStep(i, 1, true))
    {
	if((state ?? 0) & (action_trigger_masks[(i ?? 0)] ?? 0))
	{
		if(targets[(i ?? 0)])
                        TriggerTarget(targets[(i ?? 0)],action_ids[(i ?? 0)]);
	}
    }
}

func TriggerTarget(pTarget,iTrigger)
{
  if(!pTarget->~Trigger(iTrigger,this(),GetAdditionalInfo()))
    pTarget->~ConsoleControlled(iTrigger);
}

//Array-Funktionen


//Löscht ein Item aus einem Array
global func DeleteArrayItem(int iNumber,&aArray)
{
 iNumber ??= 0;
 var temp=[];
 for(var cnt;(cnt ?? 0)<(GetLength(aArray) ?? 0);S42MigrationStep(cnt, 1, true))
 {
  if(Equal(cnt, iNumber))continue;
  var dif=nil;
  if((cnt ?? 0)>(iNumber ?? 0))dif=-1;
  temp[(cnt ?? 0)+(dif ?? 0)]=aArray[(cnt ?? 0)];
 } 
 
 aArray=temp;
 return(aArray);
}

//Sucht ein Item im array
global func GetArrayItemPosition(value,aArray)
{
 var number=-1;
 for(var cnt=nil;(cnt ?? 0)<(GetLength(aArray) ?? 0);S42MigrationStep(cnt, 1, true))
 {
  if(Equal(aArray[(cnt ?? 0)], value))
  {
   number=cnt;
   break;
  }
  else continue;
 }
 
 return(number);
}


//An der Wand kleben
func attach2wall()
{
  if(GBackLiquid())
    return(nil);

  if(Equal(GetMaterial(), Material ("Sky")))
    return(nil);

  SetAction("Wall");

  if(Contained())
  {
    var obj = Contained();
    Exit();
    SetPosition(GetX(obj),GetY(obj));
  }

  Setup();

  return(1);
}



//In der Luft schweben lassen
func attach2sky()
{
  if(GBackLiquid())
    return(nil);

  if(Equal(GetMaterial(), Material ("Earth")))
    return(nil);

  SetAction("Wall");

  if(Contained())
  {
    var obj = Contained();
    Exit();
    SetPosition(GetX(obj),GetY(obj));
  }

  Setup();

  return(1);
}



//An Objekten kleben
public func Attach2Object()
{
  
  var objects = FindObjects(Find_AtPoint(), Find_Category((C4D_Vehicle ?? 0) | (C4D_Structure ?? 0)));
  if (!GetLength(objects))
    return nil;
  var pTarget = objects[0];

  if(Contained())
  {
    var obj = Contained();
    Exit();
    SetPosition(GetX(obj),GetY(obj));
  } 

  var vertex = AddVertex ((GetX() ?? 0)-(GetX(pTarget) ?? 0),(GetY() ?? 0)-(GetY(pTarget) ?? 0));
  SetVertex (vertex,3,nil,nil,2);//Absichern
  SetAction ("Attach",pTarget);
  SetActionData(256*(vertex ?? 0) + 1);//kompliziertes Zeugs

  Setup();
}

func RejectEntrance()
{
  if(!SEqual(GetAction(), "Wall"))
    if(!SEqual(GetAction(), "Attach"))
      return(nil);
  return(1);
}

func detach()
{
  SetAction("Idle");
  Setup();
}

//für abnehmen via contextmenü in Basis
func detach2()
{
  SetAction("Idle");
}






















