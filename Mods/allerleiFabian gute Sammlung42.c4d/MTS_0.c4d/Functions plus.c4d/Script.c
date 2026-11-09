/*-- Neues Objekt --*/

#strict

global func AddLeerItem(repeat,target)//fügt ein leeres AddMenuItem hinzu
{
if(repeat<=0)repeat=1;
var a;
for(a=1;a<=repeat;a++)
 AddMenuItem("","",QQQQ,target);
return(1);
}

global func DrawLightningx(x0, y0, x1, y1)//lässt ein Blitz enstehen(welcher in SE schon vorher vorhanden war)
{
  //Bemerkung, SetCon kann null werden was zum zerstören des Objektes führt.
  var lgh = CreateObject(SLGH,0,0,-1);
  SetCon(Distance(x0,y0,x1,y1)/2 , lgh);
  SetR(Angle(x0,y0,x1,y1), lgh);
  if(lgh)SetPosition(GetX()+((x0+x1)/2), GetY()+((y0+y1)/2), lgh);
  return(lgh);
}

//findet ein Objekt in eine bestimmte richtung
global func FindObjectCircleDiagonal(angle,startradius,endradius,obj,friendly,player,id1,id2,id3)//friendly und player noch nicht unterstüzt
{
var x,y;
x=GetX(obj);
y=GetY(obj);
var a=CreateObject(QQQQ,0,0,-1);
SetPosition(x,y,a);
var fund=0;
for(startradius;startradius<=endradius;startradius+=20)
 {
 if(GetX(a)<0)break;
 if(GetY(a)<0)break;
 if(GetX(a)>LandscapeWidth())break;
 if(GetY(a)>LandscapeHeight())break;
 SetPosition(x+Sin(angle,startradius),y-Cos(angle,startradius),a);
 if(!fund)if(id1)if(fund=FindObject(id1,(GetX(a)-GetX())-20,(GetY(a)-GetY())-20,40,40))startradius=endradius;
 if(!fund)if(id2)if(fund=FindObject(id2,(GetX(a)-GetX())-20,(GetY(a)-GetY())-20,40,40))startradius=endradius;
 if(!fund)if(id3)if(fund=FindObject(id3,(GetX(a)-GetX())-20,(GetY(a)-GetY())-20,40,40))startradius=endradius;
 if(!PathFree(GetX(),GetY(),GetX(a),GetY(a)))return(0);
 }
 RemoveObject(a);
 if(fund)return(fund);
 else return(0);
}


