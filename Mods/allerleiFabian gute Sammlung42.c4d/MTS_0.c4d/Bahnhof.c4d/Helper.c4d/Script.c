/*-- Neues Objekt --*/

#strict 3

local radius,r,mother;

func Initialize() {
r=nil;
SetVisibility(VIS_Owner);
radius=200;   
  return(1);
}


func Radius(ID,rad)
{
radius=(radius ?? 0)+(rad ?? 0);
if((radius ?? 0)<60)radius=60;
DoIt();
return(radius);
}

func DoIt()
{
SetPosition((GetX(mother) ?? 0)+(Sin(r,radius) ?? 0),(GetY(mother) ?? 0)-(Cos(r,radius) ?? 0));
PlayerMessage(GetOwner(),"@<c fbfbfb>%d° Winkel|%d Radius",this(),r,radius);
DelOldLine();
Magicway();
}

func DelOldLine()
{
var a;
while(a=FindObjectOwner(SMK1,GetOwner(),nil,nil,nil,nil,nil,nil,nil,a)) if(Equal(Local(1,a), this()))RemoveObject(a);
}

func Magicway()
{
//DelOldLine();
//DrawParticleLine("NoGravSpark",0,0,GetX(mother)-GetX(),GetY(mother)-GetY(),5,30,RGBa(200,10,10,10),RGBa(255,50,50,10));
var a,b;
for(a=nil ; (a ?? 0)<(radius ?? 0) ; S42MigrationAssign(a, "+=", 20))
{b=CreateObject(SMK1,nil,nil,GetOwner());b->SetTimer(nil);Local(1,b)=this();SetPosition((GetX(mother) ?? 0)+(Sin(r,a) ?? 0),(GetY(mother) ?? 0)-(Cos(r,a) ?? 0),b);}
}

func SetDreh(newr)
{
DelOldLine();
Magicway();
r=newr;
return(1);
}

func SetDreh2(ID,newr)
{
DelOldLine();
Magicway();
r=newr;
DoIt();
return(1);
}

func Dreh(ID,dreh)
{
r=(r ?? 0)+(dreh ?? 0);
if((r ?? 0)>360)S42MigrationAssign(r, "-=", 360);
if((r ?? 0)<-360)S42MigrationAssign(r, "+=", 360);
DoIt();
return(r);
}

func ShowSomething(ID,rad)
{
var hlp1=LocalN("gleis1",mother);
var hlp2=LocalN("gleis2",mother);
var hlp3=LocalN("gleis3",mother);
var hlp4=LocalN("gleis4",mother);
if(Equal(rad, 1)){SetDreh(hlp1);DoIt();DrawParticleLine("NoGravSpark",(GetX(mother) ?? 0)-(GetX() ?? 0),(GetY(mother) ?? 0)-(GetY() ?? 0),(GetX(mother) ?? 0)-(GetX() ?? 0)+(Sin(hlp1,200) ?? 0),(GetY(mother) ?? 0)-(GetY() ?? 0)-(Cos(hlp1,200) ?? 0),5,30,RGBa(200,10,10,10),RGBa(255,50,50,10));}
if(Equal(rad, 2)){SetDreh(hlp2);DoIt();DrawParticleLine("NoGravSpark",(GetX(mother) ?? 0)-(GetX() ?? 0),(GetY(mother) ?? 0)-(GetY() ?? 0),(GetX(mother) ?? 0)-(GetX() ?? 0)+(Sin(hlp2,200) ?? 0),(GetY(mother) ?? 0)-(GetY() ?? 0)-(Cos(hlp2,200) ?? 0),5,30,RGBa(200,10,10,10),RGBa(255,50,50,10));}
if(Equal(rad, 3)){SetDreh(hlp3);DoIt();DrawParticleLine("NoGravSpark",(GetX(mother) ?? 0)-(GetX() ?? 0),(GetY(mother) ?? 0)-(GetY() ?? 0),(GetX(mother) ?? 0)-(GetX() ?? 0)+(Sin(hlp3,200) ?? 0),(GetY(mother) ?? 0)-(GetY() ?? 0)-(Cos(hlp3,200) ?? 0),5,30,RGBa(200,10,10,10),RGBa(255,50,50,10));}
if(Equal(rad, 4)){SetDreh(hlp4);DoIt();DrawParticleLine("NoGravSpark",(GetX(mother) ?? 0)-(GetX() ?? 0),(GetY(mother) ?? 0)-(GetY() ?? 0),(GetX(mother) ?? 0)-(GetX() ?? 0)+(Sin(hlp4,200) ?? 0),(GetY(mother) ?? 0)-(GetY() ?? 0)-(Cos(hlp4,200) ?? 0),5,30,RGBa(200,10,10,10),RGBa(255,50,50,10));}
}

func Feldweicheplatz(ID,cont)
{
var a=CreateConstruction(ID,nil,+10,GetOwner(cont),100);
CloseMenu(cont);
Contained(cont)->WegSetWeich(ID,a);
DelOldLine();
RemoveObject();
}

