/*-- Neues Objekt --*/

#strict
#appendto FXL1
#include FXL1

private func Advance()
{
  // Einschlag
  var iVtx = GetVertexNum()-1, iVtxX = GetVertex(iVtx, 0), iVtxY = GetVertex(iVtx, 1);
  if (GBackSemiSolid(iVtxX-GetX(), iVtxY-GetY() ))
    return(Remove());

  // Objektschaden
  if (iVtx>5) BlastObjects(iVtxX, iVtxY, 5);

  // Verzweigung
  if (!Random(35))
    LaunchLightning(iVtxX, iVtxY, iAdvX, iVarX, iAdvY, iVarY);

  // Neuen Eckpunkt
  if (!AddVertex( iVtxX+iAdvX+Random(iVarX), iVtxY+iAdvY+Random(iVarY)))
    return(Remove());

  // Objektanziehung
  iVtx = GetVertexNum()-1; iVtxX = GetVertex(iVtx, 0); iVtxY = GetVertex(iVtx, 1);
  var obj;
  if (iVtx>7)
    if (obj = FindObject( 0, iVtxX-GetX()-50, iVtxY-GetY()-50, 100, 100, OCF_AttractLightning(), 0,0, NoContainer()  ) )
      Attraction(obj);

  LightningEffect(4);


  for(var pClonk in FindObjects(Find_AtPoint(iVtxX-51, iVtxY-46),Find_OCF(OCF_CrewMember))) pClonk->~LightningBlast(RandomX(10,20));

  // Weiter	
  return(ExecLgt());
}
