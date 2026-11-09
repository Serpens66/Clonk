#strict

local alpha,tick;

func Destruct(){ return(alpha+=10);}

func NoAction() //Keine Äktschn!
{
 if(!ActIdle(GetActionTarget())) ObjectSetAction(GetActionTarget(),"Idle");
 Extinguish(GetActionTarget());
 if(alpha) {Destructing();return(1);}


// wenn etwas in der Nähe brennt, oder Fackel oder lagerfeuer, schnelleres tauen.
var feuer;
if(feuer = FindObject2(Find_Distance(80),Find_Or(Find_OCF(OCF_OnFire),Find_And(Find_ID(LAGF),Find_Action("Burn")),Find_And(Find_ID(TORC),Find_Action("Burning")) ) ))
{tick+=20;}
else ++tick;
if(tick>=500) Destruct();
 return(1);
}

func Destructing()
{
  SetClrModulation(RGBa(255,255,255,alpha));
// wenn etwas in der Nähe brennt, oder Fackel oder lagerfeuer, schnelleres tauen.
var feuer;
if(feuer = FindObject2(Find_Distance(80),Find_Or(Find_OCF(OCF_OnFire),Find_And(Find_ID(LAGF),Find_Action("Burn")),Find_And(Find_ID(TORC),Find_Action("Burning")) ) ))
{alpha+=15;}
 else alpha+=1;
  if(alpha>120) {ObjectSetAction(GetActionTarget(),"Walk"); RemoveObject();}
  return(1);
}



