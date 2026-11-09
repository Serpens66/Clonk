/*-- Neues Objekt --*/

#strict 3
#appendto FXL1
#include FXL1

private func Advance()
{
  // Einschlag
  var iVtx = (GetVertexNum() ?? 0)-1, iVtxX = GetVertex(iVtx, nil), iVtxY = GetVertex(iVtx, 1);
  if (GBackSemiSolid((iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0) ))
    return(Remove());

  // Objektschaden
  if ((iVtx ?? 0)>5) BlastObjects(iVtxX, iVtxY, 5);

  // Verzweigung
  if (!Random(35))
    LaunchLightning(iVtxX, iVtxY, iAdvX, iVarX, iAdvY, iVarY);

  // Neuen Eckpunkt
  if (!AddVertex( (iVtxX ?? 0)+(iAdvX ?? 0)+(Random(iVarX) ?? 0), (iVtxY ?? 0)+(iAdvY ?? 0)+(Random(iVarY) ?? 0)))
    return(Remove());

  // Objektanziehung
  iVtx = (GetVertexNum() ?? 0)-1; iVtxX = GetVertex(iVtx, nil); iVtxY = GetVertex(iVtx, 1);
  var obj;
  if ((iVtx ?? 0)>7)
    if (obj = FindObject( nil, (iVtxX ?? 0)-(GetX() ?? 0)-50, (iVtxY ?? 0)-(GetY() ?? 0)-50, 100, 100, OCF_AttractLightning, nil,nil, NoContainer()  ) )
      Attraction(obj);

  LightningEffect(4);


  for(var pClonk in FindObjects(Find_AtPoint((iVtxX ?? 0)-51, (iVtxY ?? 0)-46),Find_OCF(OCF_CrewMember))) pClonk->~LightningBlast(RandomX(10,20));

  // Weiter	
  return(ExecLgt());
}
