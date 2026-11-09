#strict 3

 func Activate()
  {
  if(Local(nil)) return(Sound("Error"));
  if(SEqual(GetAction(),"Test")) return(Sound("Error")); 
  if(Hostile(GetOwner(),GetOwner(Par(nil))))
    return(And(Message("kein Zugriff",Par(nil)),Sound("Error")));
  if(SEqual(GetAction(),"Zeit")) return(Stop());
  SetAction("Zeit");
  return(1);
  }

 func Zeit()
  {
  if(Not(SEqual(GetAction(),"Zeit"))) return(nil);
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
  if(Hostile(GetOwner(),GetOwner(Par(nil))))
    return(And(Message("kein Zugriff",Par(nil)),Sound("Error")));
  CreateMenu(B15B,Par(nil));
  AddMenuItem("%s","Modus1",B25B,Par(nil));
  AddMenuItem("%s","Modus2",B35B,Par(nil));
  AddMenuItem("%s","Modus3",B45B,Par(nil));
  if(Equal(Local(1),nil)) AddMenuItem("%s einschalten","Modus4",B55B,Par(nil));
  if(Equal(Local(1),1)) AddMenuItem("%s ausschalten","Modus4",B55B,Par(nil));
  return(1);
  }

 func Modus1()
  {
  SetLocal(nil);
  if(SEqual(GetAction(),"Test")) SetAction("Auswahl");
  Sound("Modus");
  return(1);
  }

 func Modus2()
  {
  SetLocal(nil,1);
  SetLocal(1,nil);
  if(SEqual(GetAction(),"Zeit")) Stop();
  if(SEqual(GetAction(),"Test")) SetAction("Auswahl");
  Sound("Modus");
  return(1);
  }

 func Modus3()
  {
  SetLocal(nil);
  if(SEqual(GetAction(),"Zeit")) Stop();
  SetAction("Test");
  Sound("Modus");
  return(1);
  }

 func Modus4()
  {
  if(Local(nil)) return(Sound("Error"));
  if(Equal(Local(1),nil)) return(And(SetLocal(1,1),Sound("Modus")));
  if(Equal(Local(1),1)) return(And(SetLocal(1,nil),Sound("Modus")));
  return(1);
  }

 func Test()
  {
  while (SetVar(nil,FindObject(nil,-10,-10,20,20,OCF_Living,nil,nil,nil,Var(nil))))
    if(PathFree(GetX(),GetY(),GetX(Var(nil)),GetY(Var(nil))))
      if(Not(BitAnd(GetOCF(Var(nil)),OCF_CrewMember)))
        if(Not(Equal(GetID(Var(nil)),ZAP1)))
          if(GetAlive(Var(nil)))
            if(Containertest(Var(nil))) return(Bumm());
  while (SetVar(nil,FindObject(nil,-10,-10,20,20,OCF_CrewMember,nil,nil,nil,Var(nil))))
    if(Hostile(GetOwner(),GetOwner(Var(nil))))
      if(Containertest(Var(nil))) return(Bumm());
  return(1);
  }

 func Containertest()
  {
  if(Contained(Contained()))                             return(nil);
  if(Not(Contained()))                                   return(1);
  if(BitAnd(GetOCF(Contained()),OCF_Living))           return(1);
  if(Equal(Contained(),Contained(Par(nil))))               return(1);
  if(Equal(GetController(Contained()),GetOwner(Par(nil)))) return(1);
  if(Equal(Contained(),GetActionTarget(nil,Par(nil))))	 return(1);
  return(nil);
  }

 func Entrance()
  {
  if(Not(Local(1))) return(nil);
  SetCategory(2576);
  while (Not(Equal(Contents(nil,Par(nil)),this())))
    ShiftContents(Par(nil));
  return(1);
  }

 func Hit()
  {
  if(Local(nil)) return(Bumm());
  if(Local(1)) SetCategory(1);
  Sound("MetalHit*");
  return(1);
  }


 func Bumm()
{
  if(BitAnd(GetOCF(Contained()),OCF_Living)) DoEnergy(-100,Contained());
  if(Contained()) Exit();
  for(var i=nil;(i ?? 0)<5;S42MigrationStep(i, 1)) {if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(20,CreateObject(WIPF,nil,-3)); else FakeExplode(20,CreateObject(WIPF,nil,-3),nil,20);}
  var gate; 
  while (gate=FindObject(GATE,-60,-20,120,40, nil, nil,nil, NoContainer(), gate)) DoDamage(250,gate);
  while (gate=FindObject(GAT2,-60,-30,120,60, nil, nil,nil, NoContainer(), gate)) DoDamage(250,gate);
  while (gate=FindObject(GAT3,-60,-30,120,60, nil, nil,nil, NoContainer(), gate)) DoDamage(250,gate);
  SetComponent(BOMB,61);
  SetComponent(BOM2,31);
  ObjectSetAction(CreateObject(DT3V),"Sprenger",this());
  return(CenterExplode());
}

 func CenterExplode()
  {
  ObjectCall(CreateObject(EY3V,nil,nil,GetOwner()),"Launch",50,6);
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(nil,15)),Sum(GetY(),Mul(-1,Cos(nil,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(nil,15),Sum(Mul(-1,Cos(nil,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),nil),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
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
  if(!(Equal(GetComponent(BOMB), 60))) return(SetAction("BOOM"));
  return(CenterExplode());
  }

 func Destroy()
  {
  if(Not(Equal(GetComponent(BOM2),GetComponent(BOMB)))) return(SetComponent(BOM2,GetComponent(BOMB)));
  if(Not(GetComponent(BOMB))) return(Finished());
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(nil,15)),Sum(GetY(),Mul(-1,Cos(nil,15)))))
  And(ObjectCall(CreateObject(EY3V,Sin(nil,15),Sum(Mul(-1,Cos(nil,15)),15),GetOwner()),"Launch2",50,Div(GetComponent(BOMB),12),this(),nil),SetComponent(BOMB,Sub(GetComponent(BOMB),1)));
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
  if(FindObject(DT3V,nil,nil,nil,nil,nil,"Sprenger",this())) RemoveObject(FindObject(DT3V,nil,nil,nil,nil,nil,"Sprenger",this()));
  if(FindObject(DT3V,nil,nil,nil,nil,nil,"ClearData",this())) RemoveObject(FindObject(DT3V,nil,nil,nil,nil,nil,"ClearData",this()));
  RemoveObject();
  return(1);
  }

 func IstEkePackZerbrechlich()
  {
  return(1);
  }

 func Entrance(pContainer) { SetOwner(GetOwner(pContainer)); return; }
