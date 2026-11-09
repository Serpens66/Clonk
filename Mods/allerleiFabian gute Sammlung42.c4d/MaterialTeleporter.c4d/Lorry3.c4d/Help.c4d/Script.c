/*-- Helper --*/

#strict 3

/*
local
0 parent
1 X-Pos
6 Y-Pos
2 Breite
3 Tiefe
10 AN/AUS
12 Timer

11 Temporäre An-Zeit
*/

func Initialize(){
SetAction("Aus");
SetVisibility(VIS_None);
return(1);}

func Zeige(){
Pruefen();
ShowReckt();
return(1);}

func Pruefen(){
if(Equal(Local(10), 1)){SetAction("An");}
if(Equal(Local(10), nil)){SetAction("Aus");}
if((Local(11) ?? 0)>0){
SetAction("An");
S42MigrationStep(Local(11), -1, true);
if(Local(nil))return(1);
if(Equal(Local(10,Local(nil)), this()))return(1);
}
//return(RemoveObject());}
return(1);}

func ShowReckt(){
if((Local(12) ?? 0)>0){S42MigrationStep(Local(12), -1, true); return(1);}
Local(12)=2;
DrawParticleLine("Mark",(Local(1) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)-(GetY() ?? 0),(Local(1) ?? 0)+(Local(2) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)-(GetY() ?? 0),20,20,RGB(255,255,nil),RGB(255,255,nil),nil);
DrawParticleLine("Mark",(Local(1) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)+(Local(3) ?? 0)-(GetY() ?? 0),(Local(1) ?? 0)+(Local(2) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)+(Local(3) ?? 0)-(GetY() ?? 0),20,20,RGB(255,255,nil),RGB(255,255,nil),nil);

DrawParticleLine("Mark",(Local(1) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)-(GetY() ?? 0),(Local(1) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)+(Local(3) ?? 0)-(GetY() ?? 0),20,20,RGB(255,255,nil),RGB(255,255,nil),nil);
DrawParticleLine("Mark",(Local(1) ?? 0)+(Local(2) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)-(GetY() ?? 0),(Local(1) ?? 0)+(Local(2) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)+(Local(3) ?? 0)-(GetY() ?? 0),20,20,RGB(255,255,nil),RGB(255,255,nil),nil);

CreateParticle("Mark",(Local(1) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)-(GetY() ?? 0),nil,nil,50,RGB(255,nil,nil));
CreateParticle("Mark",(Local(1) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)+(Local(3) ?? 0)-(GetY() ?? 0),nil,nil,50,RGB(255,nil,nil));

CreateParticle("Mark",(Local(1) ?? 0)+(Local(2) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)-(GetY() ?? 0),nil,nil,50,RGB(255,nil,nil));
CreateParticle("Mark",(Local(1) ?? 0)+(Local(2) ?? 0)-(GetX() ?? 0),(Local(6) ?? 0)+(Local(3) ?? 0)-(GetY() ?? 0),nil,nil,50,RGB(255,nil,nil));
return(1);}
