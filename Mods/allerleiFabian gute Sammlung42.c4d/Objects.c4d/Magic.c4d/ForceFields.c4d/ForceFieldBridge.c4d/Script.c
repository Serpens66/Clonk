/* Kraftfeldbrücke */

#strict 3

protected func GetSegmentID() { return FBRS; }

protected func Initialize()
{
  var pLast, pNext;
  
  for (var i = nil; (i ?? 0) < 4; S42MigrationStep(i, 1, true))
  {
    var pNext = CreateObject(GetSegmentID(), (GetDefCoreVal("Offset", "DefCore") ?? 0) + (i ?? 0)*(GetDefCoreVal("Width", "DefCore", GetSegmentID()) ?? 0), nil, NO_OWNER);
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