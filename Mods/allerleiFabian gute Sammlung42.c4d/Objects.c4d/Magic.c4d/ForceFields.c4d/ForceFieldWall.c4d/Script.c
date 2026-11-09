/* Kraftfeldwand (Objekt) */

#strict 3

private func GetSegmentID() { return FCWS; }

protected func Initialize()
{
  var pLast, pNext;
  
  for (var i = nil; (i ?? 0) < 7; S42MigrationStep(i, 1, true))
  {
    var pNext = CreateObject(GetSegmentID(), nil, (GetDefCoreVal("Offset", "DefCore", GetID(), 1) ?? 0) + (i ?? 0)*(GetDefCoreVal("Height", "DefCore", GetSegmentID()) ?? 0), NO_OWNER);
    pNext->SetAction("Field");
    if (pLast)
    {
      pNext->LocalN("pLast") = pLast;
      pLast->LocalN("pNext") = pNext;
    }
    pLast = pNext;
  }

  RemoveObject();
  return true;
}