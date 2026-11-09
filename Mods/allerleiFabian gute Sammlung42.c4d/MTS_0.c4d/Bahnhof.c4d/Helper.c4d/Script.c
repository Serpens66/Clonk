/*-- Neues Objekt --*/

#strict

local radius,r,mother;

func Initialize() {
r=0;
SetVisibility(VIS_Owner());
radius=200;   
  return(1);
}


func Radius(ID,rad)
{
radius=radius+rad;
if(radius<60)radius=60;
DoIt();
return(radius);
}

func DoIt()
{
SetPosition(GetX(mother)+Sin(r,radius),GetY(mother)-Cos(r,radius));
PlayerMessage(GetOwner(),"@<c fbfbfb>%d° Winkel|%d Radius",this(),r,radius);
DelOldLine();
Magicway();
}

func DelOldLine()
{
var a;
while(a=FindObjectOwner(SMK1,GetOwner(),0,0,0,0,0,0,0,a)) if(Local(1,a)==this())RemoveObject(a);
}

func Magicway()
{
//DelOldLine();
//DrawParticleLine("NoGravSpark",0,0,GetX(mother)-GetX(),GetY(mother)-GetY(),5,30,RGBa(200,10,10,10),RGBa(255,50,50,10));
var a,b;
for(a=0 ; a<radius ; a+=20)
{b=CreateObject(SMK1,0,0,GetOwner());b->SetTimer(0);Local(1,b)=this();SetPosition(GetX(mother)+Sin(r,a),GetY(mother)-Cos(r,a),b);}
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
r=r+dreh;
if(r>360)r-=360;
if(r<-360)r+=360;
DoIt();
return(r);
}

func ShowSomething(ID,rad)
{
var hlp1=LocalN("gleis1",mother);
var hlp2=LocalN("gleis2",mother);
var hlp3=LocalN("gleis3",mother);
var hlp4=LocalN("gleis4",mother);
if(rad==1){SetDreh(hlp1);DoIt();DrawParticleLine("NoGravSpark",GetX(mother)-GetX(),GetY(mother)-GetY(),GetX(mother)-GetX()+Sin(hlp1,200),GetY(mother)-GetY()-Cos(hlp1,200),5,30,RGBa(200,10,10,10),RGBa(255,50,50,10));}
if(rad==2){SetDreh(hlp2);DoIt();DrawParticleLine("NoGravSpark",GetX(mother)-GetX(),GetY(mother)-GetY(),GetX(mother)-GetX()+Sin(hlp2,200),GetY(mother)-GetY()-Cos(hlp2,200),5,30,RGBa(200,10,10,10),RGBa(255,50,50,10));}
if(rad==3){SetDreh(hlp3);DoIt();DrawParticleLine("NoGravSpark",GetX(mother)-GetX(),GetY(mother)-GetY(),GetX(mother)-GetX()+Sin(hlp3,200),GetY(mother)-GetY()-Cos(hlp3,200),5,30,RGBa(200,10,10,10),RGBa(255,50,50,10));}
if(rad==4){SetDreh(hlp4);DoIt();DrawParticleLine("NoGravSpark",GetX(mother)-GetX(),GetY(mother)-GetY(),GetX(mother)-GetX()+Sin(hlp4,200),GetY(mother)-GetY()-Cos(hlp4,200),5,30,RGBa(200,10,10,10),RGBa(255,50,50,10));}
}

func Feldweicheplatz(ID,cont)
{
var a=CreateConstruction(ID,0,+10,GetOwner(cont),100);
CloseMenu(cont);
Contained(cont)->WegSetWeich(ID,a);
DelOldLine();
RemoveObject();
}

