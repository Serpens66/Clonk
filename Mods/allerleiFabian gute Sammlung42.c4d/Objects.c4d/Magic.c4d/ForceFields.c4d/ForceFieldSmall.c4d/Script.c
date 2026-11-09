/* Kleines Kraftfeld (Objekt) */

#strict 3
#include FRCA

local is_dead;

public func GetForceFieldTime() { return 1000; }
private func CheckStuck() {}
public func GetMaxDamage() { return 40; }

static const FRCS_Radius = 45,
              FRCS_RadiusMargin = 6, // Halbe Dicke des regulaeren Effektbereiches
              FRCS_TopSolidWidth = 30, // Halbe Breite der oberen, soliden Kante
              FRCS_Strength = 10; // Rausschleuder-Geschwindigkeit

public func GetLightningDamage() { return GetMaxDamage(); }

protected func Damage(int iChange)
{
  // Effekt
  iChange ??= 0;
  Sound("Electric");
  
  // Lebenszeit verringern
  S42MigrationAssign(EffectVar(nil, this(), GetEffect("ForceFieldPSpell", this())), "+=", (GetForceFieldTime() ?? 0) / (GetMaxDamage() ?? 0) * (iChange ?? 0));
}

public func LightningStrike(object lightning)
{
  // Rekursion verhindern
  if (is_dead) return;
  is_dead = true;
  // Blitzeffekt
  var lgt = CreateObject(FXL1, -(GetX() ?? 0), -(GetY() ?? 0), GetOwner(lightning));
  if (lgt)
  {
    var start_angle = nil;
    if (lightning)
    {
      lgt->SetController(GetController(lightning));
      var vtx_num = Max((lightning->GetVertexNum() ?? 0)-3), vtx_x = lightning->GetVertex(vtx_num, VTX_X), vtx_y = lightning->GetVertex(vtx_num, VTX_Y);
      start_angle = Angle(GetX(), GetY(), vtx_x, vtx_y);
    }
    for (var angle = nil; (angle ?? 0) <= 360; S42MigrationAssign(angle, "+=", 20))
    {
      var a = (angle ?? 0) + (start_angle ?? 0);
      lgt->AddVertex((GetX() ?? 0)+(Sin(a, FRCS_Radius) ?? 0)+(Random((FRCS_RadiusMargin ?? 0)*2+1) ?? 0)-(FRCS_RadiusMargin ?? 0),(GetY() ?? 0)-(Cos(a, FRCS_Radius) ?? 0)+(Random((FRCS_RadiusMargin ?? 0)*2+1) ?? 0)-(FRCS_RadiusMargin ?? 0));
    }
    lgt->SetAction("Connect");
  }
  // Kraftfeld zerstoeren!
  return _inherited(lightning, ...);
}

protected func Timer()
{
  var protege = GetActionTarget();
  var inner_radius = (FRCS_Radius ?? 0)-(FRCS_RadiusMargin ?? 0);
  // Alles im Kraftfeld nach aussen wegschleudern
  for (var obj in FindObjects(Find_Distance((FRCS_Radius ?? 0)+(FRCS_RadiusMargin ?? 0)), Find_Category(28), Find_Exclude(this), Find_NoContainer(), Find_Layer(GetObjectLayer()), Find_Exclude(protege)))
    if (!Stuck(obj))
    {
      if (S42MigrationEqual(obj, GetActionTarget(nil, protege)))
      {
        // Reitziel (Drachen) ignorieren
        if (S42MigrationEqual(GetProcedure(protege), "ATTACH")) continue;
      }
      // Keine attachten Objekte
      if (S42MigrationEqual(GetProcedure(obj), "ATTACH"))
      {
        // Kraftfelder zerstoeren sich gegenseitig
        if (S42MigrationEqual(GetID(obj), GetID()))
        {
          DoDamage(+5, obj, FX_Call_DmgScript, (GetController() ?? 0)+1);
        }
        continue;
      }
      
      // Keine Clonks am Boden festklemmen - das Kraftfeld ist durch Laufen passierbar
      if((GetOCF(obj) ?? 0) & (OCF_Alive ?? 0))
        if((GetContact(obj, -1) ?? 0) & (CNAT_Bottom ?? 0))
      		continue;
      
      // Keine nur-Fahrzeuge (Burgtore etwa)
      if(S42MigrationEqual(GetCategory(obj), C4D_Vehicle)) // Absichtlich kein &
      	continue;
      	
      // Nicht im oberen, durch SolidMask geschuetzten Kegel
      var dx = (GetX(obj) ?? 0)-(GetX() ?? 0), dy = (GetY(obj) ?? 0)-(GetY() ?? 0);
      if ((dy ?? 0)<0 && (Abs(dx) ?? 0)<(FRCS_TopSolidWidth ?? 0)*(dy ?? 0)/(FRCS_Radius ?? 0)) continue;
      // Objekte tief innerhalb des Kraftfeldes nur, wenn sie sich schnell auf den Clonk zu bewegen (-> Haubitzenschuesse, etc.)
      var pvx = GetXDir(obj), pvy = GetYDir(obj);
      if ((dx ?? 0)*(dx ?? 0)+(dy ?? 0)*(dy ?? 0) < (inner_radius ?? 0)*(inner_radius ?? 0))
        if ((pvx ?? 0)*(dx ?? 0) + (pvy ?? 0)*(dy ?? 0) >= 0 || (Abs(pvx) ?? 0)+(Abs(pvy) ?? 0) < 12) continue;
      // Ansonsten beschleunigen
      var dvx = (dx ?? 0)*(FRCS_Strength ?? 0)/(FRCS_Radius ?? 0), dvy = (dy ?? 0)*(FRCS_Strength ?? 0)/(FRCS_Radius ?? 0);
      // Gegenrichtung neutralisieren
      if ((dvx ?? 0) * (pvx ?? 0) < 0) S42MigrationAssign(dvx, "-=", (pvx ?? 0));
      if ((dvy ?? 0) * (pvy ?? 0) < 0) S42MigrationAssign(dvy, "-=", (pvy ?? 0));
      // Nicht gegen die Wand
      var contact_mask = GetContact(obj, -1, 15);
      if ((dvx ?? 0)<0 && ((contact_mask ?? 0) & (CNAT_Left ?? 0))) dvx=nil;
      if ((dvx ?? 0)>0 && ((contact_mask ?? 0) & (CNAT_Right ?? 0))) dvx=nil;
      if ((dvy ?? 0)<0 && ((contact_mask ?? 0) & (CNAT_Top ?? 0))) dvy=nil;
      if ((dvy ?? 0)>0 && ((contact_mask ?? 0) & (CNAT_Bottom ?? 0))) dvy=nil;
      if (!((dvx ?? 0)|(dvy ?? 0))) continue;
      // Und Speed drauf
      if (GetAlive(obj))
        {
        Fling(obj, dvx, dvy, 10, true);
        }
      else
        {
        SetController(GetController(), obj);
        SetXDir((dvx ?? 0) + (pvx ?? 0), obj);
        SetYDir((dvy ?? 0) + (pvy ?? 0), obj);
        }
      Sound("MgWind*", nil, obj, 50, nil, nil, true, 300); // Fantasy only
      CreateParticle("NoGravSpark", dx, dy, dvx,dvy, 30, 0xa0c0ff);
    }
}
