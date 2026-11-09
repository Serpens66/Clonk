/*-- Keine Grenzen! --*/

#strict 3

protected func Initialize(){

 SetPosition();
 return(1);}

protected func Check(){
  // Links
  while(Var()=FindObject(nil,nil,nil,40,LandscapeHeight(),nil,nil,nil,nil,Var()))
    if(!((GetCategory(Var()) ?? 0)&(C4D_StaticBack ?? 0)))
      if((GetX(Var()) ?? 0)-(GetDefCoreVal("Width","DefCore",GetID(Var())) ?? 0)/2<2)
        if(Or((GetXDir(Var()) ?? 0)<0, Equal(GetComDir(Var()), COMD_Left)))
          SetPosition((LandscapeWidth() ?? 0)-(GetDefCoreVal("Width","DefCore",GetID(Var())) ?? 0)/2,GetY(Var()),Var());

  // Rechts
  while(Var()=FindObject(nil,(LandscapeWidth() ?? 0)-40,nil,40,LandscapeHeight(),nil,nil,nil,nil,Var()))
    if(!((GetCategory(Var()) ?? 0)&(C4D_StaticBack ?? 0)))
      if((GetX(Var()) ?? 0)+(GetDefCoreVal("Width","DefCore",GetID(Var())) ?? 0)/2>(LandscapeWidth() ?? 0)-2)
        if(Or((GetXDir(Var()) ?? 0)>0, Equal(GetComDir(Var()), COMD_Right)))
          SetPosition((GetDefCoreVal("Width","DefCore",GetID(Var())) ?? 0)/2,GetY(Var()),Var());

  return(1);}


// Kann mittels des neuen Spielzielauswählers gewählt werden
public func IsChooseable2() { return(1); }