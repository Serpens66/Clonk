/*-- Verbranntes Burgteil --*/

#strict 3

/* Entzündung */

protected func Incineration()
{
  return(FCastleChange());
}

// Burgteile auf ihre Daseinsberechtigung überprüfen (Brustwehren inmitten der Burg, etc.)
public func FCastleChange()
{
  // Alle Teile benachrichtigen
  var pPart;
  while(pPart = FindObject(nil, nil, nil, nil, nil, OCF_Fullcon, nil,nil, NoContainer(), pPart))
    pPart->~CastleChange();
  return(1);
}