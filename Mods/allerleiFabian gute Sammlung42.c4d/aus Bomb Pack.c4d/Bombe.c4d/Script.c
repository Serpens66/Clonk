#strict

 func Activate()
  {
  if(Local(0)) return(Sound("Error")); 
  if(SEqual(GetAction(),"Test")) return(Sound("Error")); 
  if(Hostile(GetOwner(),GetOwner(Par(0)))) 
    return(And(Message("kein Zugriff",Par(0)),Sound("Error")));
  if(SEqual(GetAction(),"Zeit")) return(Stop());
  SetAction("Zeit");
  return(1);
  }

 func Zeit()
  {
  if(Not(SEqual(GetAction(),"Zeit"))) return(0);
  SetLocal(2,Sum(Local(2),1));
  if(GreaterThan(Local(2),10)) return(Bumm());
  Message("@%d",this(),Mul(Sub(Local(2),11),-1));
  return(1);
  }

 func Stop()
  {
  SetAction("Auswahl");
  Message("Stop",this());
  return(1);
  }

 func Auswahl()
  {
  if(SEqual(GetAction(),"Zeit")) Message("Stop",this());
  SetAction("Auswahl");
  Sound("BOAuswahl");
  return(1);
  }

 func ControlSpecial2()
  {
  if(Hostile(GetOwner(),GetOwner(Par(0)))) 
    return(And(Message("kein Zugriff",Par(0)),Sound("Error")));
  CreateMenu(B15B,Par(0));
  AddMenuItem("%s","Modus1",B25B,Par(0));
  AddMenuItem("%s","Modus2",B35B,Par(0));
  AddMenuItem("%s","Modus3",B45B,Par(0));
  if(Equal(Local(1),0)) AddMenuItem("%s einschalten","Modus4",B55B,Par(0));
  if(Equal(Local(1),1)) AddMenuItem("%s ausschalten","Modus4",B55B,Par(0));
  return(1);
  }

 func Modus1()
  {
  SetLocal(0);
  if(SEqual(GetAction(),"Test")) SetAction("Auswahl");
  Sound("Modus");
  return(1);
  }

 func Modus2()
  {
  SetLocal(0,1);
  SetLocal(1,0);
  if(SEqual(GetAction(),"Zeit")) Stop();
  if(SEqual(GetAction(),"Test")) SetAction("Auswahl");
  Sound("Modus");
  return(1);
  }

 func Modus3()
  {
  SetLocal(0);
  if(SEqual(GetAction(),"Zeit")) Stop();
  SetAction("Test");
  Sound("Modus");
  return(1);
  }

 func Modus4()
  {
  if(Local(0)) return(Sound("Error"));
  if(Equal(Local(1),0)) return(And(SetLocal(1,1),Sound("Modus")));
  if(Equal(Local(1),1)) return(And(SetLocal(1,0),Sound("Modus")));
  return(1);
  }

 func Test()
  {
  while (SetVar(0,FindObject(0,-10,-10,20,20,OCF_Living(),0,0,0,Var(0)))) 
    if(PathFree(GetX(),GetY(),GetX(Var(0)),GetY(Var(0))))
      if(Not(BitAnd(GetOCF(Var(0)),OCF_CrewMember())))  
        if(Not(Equal(GetID(Var(0)),ZAP1)))  
          if(GetAlive(Var(0)))
            if(Containertest(Var(0))) return(Bumm());
  while (SetVar(0,FindObject(0,-10,-10,20,20,OCF_CrewMember(),0,0,0,Var(0)))) 
    if(Hostile(GetOwner(),GetOwner(Var(0))))
      if(Containertest(Var(0))) return(Bumm());
  return(1);
  }

 func Containertest()
  {
  if(Contained(Contained()))                             return(0);
  if(Not(Contained()))                                   return(1);
  if(BitAnd(GetOCF(Contained()),OCF_Living()))           return(1);
  if(Equal(Contained(),Contained(Par(0))))               return(1); 
  if(Equal(GetController(Contained()),GetOwner(Par(0)))) return(1);
  if(Equal(Contained(),GetActionTarget(0,Par(0))))	 return(1);
  return(0);
  }

 func Entrance()
  {
  if(Not(Local(1))) return(0);
  SetCategory(2576);
  while (Not(Equal(Contents(0,Par(0)),this())))
    ShiftContents(Par(0));
  return(1);
  }

 func Hit()
  {
  if(Local(0)) return(Bumm());
  if(Local(1)) SetCategory(1);
  Sound("MetalHit*");
  return(1);
  }


 func Bumm()
{
  if(BitAnd(GetOCF(Contained()),OCF_Living())) DoEnergy(-100,Contained());
  if(Contained()) Exit();
  for(var i=0;i<5;++i) {if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(20,CreateObject(WIPF,0,-3)); else FakeExplode(20,CreateObject(WIPF,0,-3),0,20);}
  var gate; 
  while (gate=FindObject(GATE,-60,-20,120,40, 0, 0,0, NoContainer(), gate)) DoDamage(250,gate);
  while (gate=FindObject(GAT2,-60,-30,120,60, 0, 0,0, NoContainer(), gate)) DoDamage(250,gate);
  while (gate=FindObject(GAT3,-60,-30,120,60, 0, 0,0, NoContainer(), gate)) DoDamage(250,gate);
  SetComponent(BOMB,61);
  SetComponent(BOM2,31);
  ObjectSetAction(CreateObject(DT3V),"Sprenger",this());
  return(CenterExplode());
}

 func CenterExplode()
  {
  ObjectCall(CreateObject(EY3V,0,0,GetOwner()),"Launch",50,6);
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(0,15)),Sum(GetY(),Mul(-1,Cos(0,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(0,15),Sum(Mul(-1,Cos(0,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),0),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(45,15)),Sum(GetY(),Mul(-1,Cos(45,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(45,15),Sum(Mul(-1,Cos(45,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),45),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(90,15)),Sum(GetY(),Mul(-1,Cos(90,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(90,15),Sum(Mul(-1,Cos(90,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),90),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(135,15)),Sum(GetY(),Mul(-1,Cos(135,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(135,15),Sum(Mul(-1,Cos(135,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),135),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(180,15)),Sum(GetY(),Mul(-1,Cos(180,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(180,15),Sum(Mul(-1,Cos(180,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),180),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(225,15)),Sum(GetY(),Mul(-1,Cos(225,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(225,15),Sum(Mul(-1,Cos(225,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),225),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(270,15)),Sum(GetY(),Mul(-1,Cos(270,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(270,15),Sum(Mul(-1,Cos(270,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),270),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(315,15)),Sum(GetY(),Mul(-1,Cos(315,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(315,15),Sum(Mul(-1,Cos(315,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),315),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(!(GetComponent(BOMB)==60)) return(SetAction("BOOM"));
  return(CenterExplode());
  }

 func Destroy()
  {
  if(Not(Equal(GetComponent(BOM2),GetComponent(BOMB)))) return(SetComponent(BOM2,GetComponent(BOMB)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(0,15)),Sum(GetY(),Mul(-1,Cos(0,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(0,15),Sum(Mul(-1,Cos(0,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),0),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(45,15)),Sum(GetY(),Mul(-1,Cos(45,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(45,15),Sum(Mul(-1,Cos(45,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),45),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(90,15)),Sum(GetY(),Mul(-1,Cos(90,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(90,15),Sum(Mul(-1,Cos(90,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),90),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(135,15)),Sum(GetY(),Mul(-1,Cos(135,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(135,15),Sum(Mul(-1,Cos(135,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),135),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(180,15)),Sum(GetY(),Mul(-1,Cos(180,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(180,15),Sum(Mul(-1,Cos(180,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),180),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(225,15)),Sum(GetY(),Mul(-1,Cos(225,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(225,15),Sum(Mul(-1,Cos(225,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),225),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(270,15)),Sum(GetY(),Mul(-1,Cos(270,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(270,15),Sum(Mul(-1,Cos(270,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),270),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(315,15)),Sum(GetY(),Mul(-1,Cos(315,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(315,15),Sum(Mul(-1,Cos(315,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),315),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
  return(1);
  }

 func Finished()
  {
//Systemobjekte löschen
  if(FindObject(DT3V,0,0,0,0,0,"Sprenger",this())) RemoveObject(FindObject(DT3V,0,0,0,0,0,"Sprenger",this()));
  if(FindObject(DT3V,0,0,0,0,0,"ClearData",this())) RemoveObject(FindObject(DT3V,0,0,0,0,0,"ClearData",this()));
  RemoveObject();
  return(1);
  }

 func IstEkePackZerbrechlich()
  {
  return(1);
  }

 func Entrance(pContainer) { SetOwner(GetOwner(pContainer)); return(); }
