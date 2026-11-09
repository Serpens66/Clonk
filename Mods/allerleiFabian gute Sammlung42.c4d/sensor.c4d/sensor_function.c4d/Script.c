/*-- Sensor-Functions --*/

#strict

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
if (schaden >= hp) {Incinerate();};
_inherited(a,b,c,d,e,f,g);
}



func Initialize()
{
  targets=CreateArray();
  action_ids=CreateArray();
  action_trigger_masks=CreateArray();
  last_targets=CreateArray();
  state_desc = CreateArray();
  state_selection = 0;
  state = 0;
  SetAction("Idle");
  return(1);
}

//Die Menüs
public func Activate(pCaller)
{
    if(Hostile (GetOwner(pCaller),GetOwner()))
      return(Sound ("CommandFailure1",false,pCaller,100,GetOwner(pCaller),0)); 

    pUser = pCaller;
    Setup();
}

public func ControlDigDouble(pCaller)
{
    Activate(pCaller);
} 

func Setup()
{
    CreateMenu (GetID(),pUser,0,0,0,0,1,1);
    AddMenuItem ("$ADD_TARGET$","addtarget",GetID(),pUser,0,0,"$ADD_TARGET_DESC$");
    AddMenuItem ("$DEL_TARGET$","deltarget",GetID(),pUser,0,0,"$DEL_TARGET_DESC$");

    if((GetAction() eq "Wall") || (GetAction() eq "Attach"))
      AddMenuItem ("$DETACH$","detach",GetID(),pUser,0,0,"$DETACH_DESC$");
    else
    {
      if((!GBackLiquid()) && (GetMaterial() != Material ("Sky")))
        AddMenuItem ("$ATTACH_WALL$","attach2wall",GetID(),pUser,0,0,"$ATTACH_WALL_DESC$");



      if((!GBackLiquid()) && (GetMaterial() == Material ("Sky")))
       AddMenuItem ("$ATTACH_SKY$","attach2sky",GetID(),pUser,0,0,"$ATTACH_SKY_DESC$");


// deaktiviert, weil das oft für Probleme sorgt
//      var objects = FindObjects(Find_AtPoint(), Find_Category(C4D_Vehicle | C4D_Structure));
//      if (GetLength(objects))
//      AddMenuItem ("$ATTACH_OBJECT$","Attach2Object",GetID(),pUser,0,0,"$ATTACH_OBJECT_DESC$");
    }
    ExtraSetup(0,pUser);
}

func deltarget()
{
    CreateMenu (GetID(),pUser,0,0,0,0,1,0);
    var i = 0;
    for(var obj in targets)
    {
      AddMenuItem (GetName(obj),"deltarget2",GetID(obj),pUser,0,i,"$DEL_TARGET_DESC$");
      i++;
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
        return 0;
    CreateMenu (GetID(),pUser,0,0,0,0,1,0);
    for(var i = 0;i < 
GetLength(last_targets);
i++)
    {
      if (!Hostile(last_targets[i]->GetOwner(), GetOwner()))
        AddMenuItem (GetName(last_targets[i]),Format("SetTriggerAction(%d)",i),GetID(last_targets[i]),pUser,0,0);
    }
}

func FindAttachTarget()
{
  var obj=0;
  CreateMenu(GetID(),Par(1),0,0,"$ATTACH$",0,1,0);
  while(obj=FindObject(0,-15,-80,30,90,0,0,0,NoContainer(),obj))
    AddMenuItem(Format("%s",GetName(obj)),"Toolit",GetID(obj),Par(1),0,obj,0,0,0,0);
}

func Attach(pTarget)
{
  SetAction ("Attach",pTarget);
  return(1);
}

func SetTriggerAction(a)
{
    CreateMenu (GetID(),pUser,0,0,0,0,1,0);
    var i = 1;
    while(GetType (TargetTriggerName(last_targets[a],i)) == C4V_String)
    {
        AddMenuItem (TargetTriggerName(last_targets[a],i),Format("SetTriggerState(%d,%d,0)",a,i),RSR1,pUser,0,0);
        i++;
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
    var active = 0;
    var selection = c;
    var state_pos = 0;
    CreateMenu (GetID(),pUser,0,0,0,0,1,0);
    for(var i = 0;i<GetLength(state_desc);i++)
    {
	state_pos = i;
        active = GetState(state_selection,i);
	AddMenuItem (state_desc[i],Format("SwitchState(%d,%d,%d)",a,b,i),SENS,pUser,0,0,"",2,active);
	active = 0;
    }
    AddMenuItem ("$FINISHED$",Format("AddTarget2(%d,%d,%d)",a,b,state_selection),GetID(),pUser,0,0);
    SelectMenuItem (selection,pUser);
}

func SwitchState(a,b,c)
{
	if(GetState(state_selection,c)==0)
		SetState(state_selection,c,1);
	else
	{
		if(GetState(state_selection,c)==1)
			SetState(state_selection,c,2);
		else
		{
			if(GetState(state_selection,c)==2)
				SetState(state_selection,c,0);
		}
	}
	SetTriggerState(a,b,c);
}

func ExtraSetup(){return(0);}
func ExtraMenu(){return(0);}
func GetAdditionalInfo(){return(0);}

//Bit-Funktionen
func GetBit(&bitmap,bitpos)
{
    var bit = 1 << bitpos;
    if(bitmap & bit)
        return true;
    return false;
}

func SetBit(&bitmap,bitpos,value)
{
    var bit = 1 << bitpos;
    if(value == 1)
    	bitmap = bitmap | bit;
    else
    	bitmap = bitmap & ~ bit;
}

func SwitchBit(&bitmap,bitpos)
{
    var bit = 1 << bitpos;
    bitmap = bitmap ^ bit; 
}

//Trigger-Mask
func AddState(desc)
{
    AddToArray(state_desc,desc);
}

func SetState(&bitmap,action_id,value)
{
	if(value == 0)
	{
		SetBit(bitmap,action_id*2,0);
		SetBit(bitmap,action_id*2+1,0);
		return(0);
	}
	if(value == 1)
	{
		SetBit(bitmap,action_id*2,1);
		SetBit(bitmap,action_id*2+1,0);
		return(1);
	}
	else
	{
		SetBit(bitmap,action_id*2,0);
		SetBit(bitmap,action_id*2+1,1);
		return(2);
	}
}

func GetState(&bitmap,action_id)
{
	if(GetBit(bitmap,action_id*2)==1)
		return(1);
	if(GetBit(bitmap,action_id*2+1)==1)
		return(2);
	return(0);
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
    AddTarget(last_targets[target],trigger,mask);
    state_selection = 0;
    ExtraMenu();
}

private func AddToArray(&a, add)
{
  a[GetLength(a)] = add;
}

func ResetArray(&a)
{
    SetLength(a,0);
    a = CreateArray();
}

//Triggering
func TriggerActions()
{
    for(var i = 0;i < GetLength(targets);i++)
    {
	if(state & action_trigger_masks[i])
	{
		if(targets[i])
            		TriggerTarget(targets[i],action_ids[i]);
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
 var temp=[];
 for(var cnt;cnt<GetLength(aArray);cnt++)
 {
  if(cnt==iNumber)continue;
  var dif=0;
  if(cnt>iNumber)dif=-1;
  temp[cnt+dif]=aArray[cnt];
 } 
 
 aArray=temp;
 return(aArray);
}

//Sucht ein Item im array
global func GetArrayItemPosition(value,aArray)
{
 var number=-1;
 for(var cnt=0;cnt<GetLength(aArray);cnt++)
 {
  if(aArray[cnt] == value)
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
    return(0);

  if(GetMaterial() == Material ("Sky"))
    return(0);

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
    return(0);

  if(GetMaterial() == Material ("Earth"))
    return(0);

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
  
  var objects = FindObjects(Find_AtPoint(), Find_Category(C4D_Vehicle | C4D_Structure));
  if (!GetLength(objects))
    return 0;
  var pTarget = objects[0];

  if(Contained())
  {
    var obj = Contained();
    Exit();
    SetPosition(GetX(obj),GetY(obj));
  } 

  var vertex = AddVertex (GetX()-GetX(pTarget),GetY()-GetY(pTarget));
  SetVertex (vertex,3,0,0,2);//Absichern
  SetAction ("Attach",pTarget);
  SetActionData(256*vertex + 1);//kompliziertes Zeugs

  Setup();
}

func RejectEntrance()
{
  if(GetAction() ne "Wall")
    if(GetAction() ne "Attach")
      return(0);
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






















