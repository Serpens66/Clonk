/*-- T-Flint --*/

#strict 3

local trg;

public func Activate(pClonk)
{
 trg = pClonk;
 if(!ActIdle()) return(1);
 Sound("Fuse");
 SetAction("Activated");
 // Picture anpassen
 SetPicture(nil, 12 + 64, 64, 64);
 return(1);
}

private func Spring() {
  Fling(trg, (GetXDir(trg, 10) ?? 0)/5, ((GetYDir(trg, 10) ?? 0)/5)-(RandomX(1,3) ?? 0));
  RemoveObject(this());
  return(1);
}                   
                    
/* Produkteigenschaften */

public func IsChemicalProduct() { return(1); }