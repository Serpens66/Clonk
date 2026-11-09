/*-- Neues Objekt --*/

#strict 3

global func AddLeerItem(repeat,target)//fügt ein leeres AddMenuItem hinzu
{
if((repeat ?? 0)<=0)repeat=1;
var a;
for(a=1;(a ?? 0)<=(repeat ?? 0);S42MigrationStep(a, 1, true))
 AddMenuItem("","",QQQQ,target);
return(1);
}

global func DrawLightningx(x0, y0, x1, y1)//lässt ein Blitz enstehen(welcher in SE schon vorher vorhanden war)
{
  //Bemerkung, SetCon kann null werden was zum zerstören des Objektes führt.
  var lgh = CreateObject(SLGH,nil,nil,-1);
  SetCon((Distance(x0,y0,x1,y1) ?? 0)/2 , lgh);
  SetR(Angle(x0,y0,x1,y1), lgh);
  if(lgh)SetPosition((GetX() ?? 0)+(((x0 ?? 0)+(x1 ?? 0))/2), (GetY() ?? 0)+(((y0 ?? 0)+(y1 ?? 0))/2), lgh);
  return(lgh);
}

//findet ein Objekt in eine bestimmte richtung
global func FindObjectCircleDiagonal(angle,startradius,endradius,obj,friendly,player,id1,id2,id3)//friendly und player noch nicht unterstüzt
{
var x,y;
x=GetX(obj);
y=GetY(obj);
var a=CreateObject(QQQQ,nil,nil,-1);
SetPosition(x,y,a);
var fund=nil;
for(startradius;(startradius ?? 0)<=(endradius ?? 0);S42MigrationAssign(startradius, "+=", 20))
 {
 if((GetX(a) ?? 0)<0)break;
 if((GetY(a) ?? 0)<0)break;
 if((GetX(a) ?? 0)>(LandscapeWidth() ?? 0))break;
 if((GetY(a) ?? 0)>(LandscapeHeight() ?? 0))break;
 SetPosition((x ?? 0)+(Sin(angle,startradius) ?? 0),(y ?? 0)-(Cos(angle,startradius) ?? 0),a);
 if(!fund)if(id1)if(fund=FindObject(id1,((GetX(a) ?? 0)-(GetX() ?? 0))-20,((GetY(a) ?? 0)-(GetY() ?? 0))-20,40,40))startradius=endradius;
 if(!fund)if(id2)if(fund=FindObject(id2,((GetX(a) ?? 0)-(GetX() ?? 0))-20,((GetY(a) ?? 0)-(GetY() ?? 0))-20,40,40))startradius=endradius;
 if(!fund)if(id3)if(fund=FindObject(id3,((GetX(a) ?? 0)-(GetX() ?? 0))-20,((GetY(a) ?? 0)-(GetY() ?? 0))-20,40,40))startradius=endradius;
 if(!PathFree(GetX(),GetY(),GetX(a),GetY(a)))return(nil);
 }
 RemoveObject(a);
 if(fund)return(fund);
 else return(nil);
}


