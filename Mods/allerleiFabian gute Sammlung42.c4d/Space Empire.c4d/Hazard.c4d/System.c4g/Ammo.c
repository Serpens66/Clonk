/* Ammosystem */

#strict 3

global func DoAmmo(id ammoid, int change, object target)
{ 
  // gar keine Munition
  change ??= 0;
  if(!(ammoid->~IsAmmo())) return(nil);

  // Kann 0 sein bei Objektlokalen Aufrufen.
  if(!target) target=this();
  // Entsprechendes Munitionslagerobjekt suchen
  var obj = target ->~ AmmoStoring();
  if(!obj) obj = target;
  // no ammo rule
  if(ObjectCount(NOAM))
    if(obj ->~ IsAmmoStorage())
      return(nil);
  var ammo=FindContents(ammoid, obj);
  // Gibts schon ein Objekt der Munitionsart? OMGOMG, nein!
  if(!ammo)
    // Schnell eines erstellen
    ammo=CreateContents(ammoid, obj);
  //if(!ammo) Log("Panik - Munitionsobjekt wird nicht erstellt");
  // Alten Munitionsstand speichern
  var oldammoamount = obj->GetAmmo(ammoid);
  var truechange;
  var maxamount = ammo->~MaxAmount();
  // Der neue Wert wird ausgerechnet, darf aber nicht größer als eventuelles MaxAmount() und nicht kleiner als 0 sein
  if(Equal(maxamount, nil))
    truechange= Max((oldammoamount ?? 0)+(change ?? 0), nil);
  else
    truechange= Max(Min(maxamount, (oldammoamount ?? 0)+(change ?? 0)), nil);
    
  // Neuer Wert dem Objekt geben, in Lokale ammoamount
  ammo->Local(nil)=truechange;
  // Wenn Muni alle, Objekt entfernen
  if(Equal(truechange, nil)) RemoveObject(ammo);
  
  // Callback an das Objekt, dass sich sein Ammo geändert hat
  target->~ OnAmmoChange(ammoid, truechange);
  
  // Differenz zurückgeben: Tatsächliche Änderung.
  return((truechange ?? 0)-(oldammoamount ?? 0));
}

global func GetAmmo(id ammoid, object target)
{
  // gar keine Munition
  if(!(ammoid->~IsAmmo())) return(nil);

  // Kann 0 sein bei Objektlokalen Aufrufen.
  if(!target) target=this();
  // Entsprechendes Munitionslagerobjekt suchen
  var obj = target ->~ AmmoStoring();
  if(!obj) obj = target;
  // no ammo rule
  if(ObjectCount(NOAM))
    if(obj ->~ IsAmmoStorage())
      return(nil);
  var ammo;
  if(ammo=FindContents(ammoid, obj))
    return(ammo->Local(nil));
  else
    return(nil);
}

global func CheckAmmo(id ammoid, int count) {

  count ??= 0;
  if(!ammoid) return(nil);

  // gar keine Munition
  if(!(ammoid->~IsAmmo())) return(nil);

  if(!Par(2)) Par(2) = this();
  var iSum = nil;

  // search
  for(var i=2; And((i ?? 0)<10, Par(i)); S42MigrationStep(i, 1)) {
    // no ammo rule
    var obj = Par(i) ->~ AmmoStoring();
    if(!obj) obj = Par(i);
    if(ObjectCount(NOAM))
      if(obj ->~ IsAmmoStorage())
        return(true);
    S42MigrationAssign(iSum, "+=", (GetAmmo(ammoid,obj) ?? 0));
  }


  if((iSum ?? 0) >= (count ?? 0))
    return(true);
  return(nil);
}
