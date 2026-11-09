/*-- Helper --*/

#strict

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
if(Local(10)==1){SetAction("An");}
if(Local(10)==0){SetAction("Aus");}
if(Local(11)>0){
SetAction("An");
Local(11)--;
if(Local(0))return(1);
if(Local(10,Local(0))==this())return(1);
}
//return(RemoveObject());}
return(1);}

func ShowReckt(){
if(Local(12)>0){Local(12)--; return(1);}
Local(12)=2;
DrawParticleLine("Mark",Local(1)-GetX(),Local(6)-GetY(),Local(1)+Local(2)-GetX(),Local(6)-GetY(),20,20,RGB(255,255,0),RGB(255,255,0),0);
DrawParticleLine("Mark",Local(1)-GetX(),Local(6)+Local(3)-GetY(),Local(1)+Local(2)-GetX(),Local(6)+Local(3)-GetY(),20,20,RGB(255,255,0),RGB(255,255,0),0);                                    

DrawParticleLine("Mark",Local(1)-GetX(),Local(6)-GetY(),Local(1)-GetX(),Local(6)+Local(3)-GetY(),20,20,RGB(255,255,0),RGB(255,255,0),0);
DrawParticleLine("Mark",Local(1)+Local(2)-GetX(),Local(6)-GetY(),Local(1)+Local(2)-GetX(),Local(6)+Local(3)-GetY(),20,20,RGB(255,255,0),RGB(255,255,0),0); 

CreateParticle("Mark",Local(1)-GetX(),Local(6)-GetY(),0,0,50,RGB(255,0,0));
CreateParticle("Mark",Local(1)-GetX(),Local(6)+Local(3)-GetY(),0,0,50,RGB(255,0,0));

CreateParticle("Mark",Local(1)+Local(2)-GetX(),Local(6)-GetY(),0,0,50,RGB(255,0,0));
CreateParticle("Mark",Local(1)+Local(2)-GetX(),Local(6)+Local(3)-GetY(),0,0,50,RGB(255,0,0));
return(1);}
