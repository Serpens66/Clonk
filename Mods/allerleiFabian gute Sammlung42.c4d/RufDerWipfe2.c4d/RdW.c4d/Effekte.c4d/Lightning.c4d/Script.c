/*-- Blitz --*/

#strict 3

static lgt_enlightment; // Globale Erhellung durch Blitze
static lgt_last_lgt_frame; // Letztes Frame, in dem die Helligkeit angepasst wurde

/* Locals */

local iAdvX, iVarX, // Bewegung / Abweichung X
      iAdvY, iVarY; // Bewegung / Abweichung Y
local fDoGamma;

/* Aktivierung */

public func Activate(iStartX, iStartY, iAdvanceX, iRandomX, iAdvanceY, iRandomY, fApplyGamma)
{
  // Startposition
  AddVertex(iStartX,iStartY);
  // Bewegungsverhalten
  iAdvX=iAdvanceX; iVarX=iRandomX;
  iAdvY=iAdvanceY; iVarY=iRandomY;
  fDoGamma=fApplyGamma;
  // Aktion setzen
  SetAction("Advance");
  // Geräusch
  if (!Random(5)) Sound("Thunder*");
  // Leuchteffekt
  LightningEffect(10);
  
  SetLocal(1,23);
  return(1);
}

/* Bewegung */

private func Advance()
{
  // Einschlag
  var iVtx = (GetVertexNum() ?? 0)-1, iVtxX = GetVertex(iVtx, nil), iVtxY = GetVertex(iVtx, 1);
  if (GBackSolid((iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0) ))
    return(Remove());

  // Im Wasser?
  if (GBackLiquid((iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0) ))
  {
	  var obj;
	  // Alle Lebewesen in der Nähe bekommen was ab
	  while (obj = FindObject( nil, (iVtxX ?? 0)-(GetX() ?? 0)-400, (iVtxY ?? 0)-(GetY() ?? 0)-400, 800, 800, OCF_Alive, nil,nil, NoContainer(),obj  ) )
    	  {
      	    if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
	    if(!GBackLiquid((GetX(obj) ?? 0)-(GetX() ?? 0), (GetY(obj) ?? 0)-(GetY() ?? 0))) continue;
  	    obj->~LightningStrike();
  	    Punch(obj, 12);
            DrawParticleLine ("LightningSpark", (iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0),
		    (GetX(obj) ?? 0)-(GetX() ?? 0), (GetY(obj) ?? 0)-(GetY() ?? 0), 6, 60, RGB(nil,100,255));
            DrawParticleLine ("LightningSpark", (iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0),
		    (GetX(obj) ?? 0)-(GetX() ?? 0), (GetY(obj) ?? 0)-(GetY() ?? 0),  3, 25, RGB(250,250,255));
          }
	  return(Remove());
  }

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

  //
  if(Or(Or(IsAntiLag(), FindObject2(Find_ID(ANL_))), FindObject2(Find_ID(KOHE))))
    LightningEffect(4);
  else
    if(!Random(4))
      CreateParticle("LightningSpark2",(iVtxX ?? 0)-(GetX() ?? 0),(iVtxY ?? 0)-(GetY() ?? 0),nil,nil,RandomX(2000,BoundBy(7200-(GetActTime() ?? 0)/3,6000)),RGBa(50,50,50));

  // Weiter	
  return(ExecLgt());
}

/* Effekte */

private func Sparkle()
{
  var iVtx = (GetVertexNum() ?? 0)-1, iVtxX = GetVertex(iVtx, nil), iVtxY = GetVertex(iVtx, 1);
  CastObjects(SPRK,1,20,(iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0));
  ExecLgt();
  return(1);
}

private func LightningEffect(strength)
{
  S42MigrationAssign(lgt_enlightment, "+=", (Random(strength) ?? 0)+(strength ?? 0)/2);
  return(1);
}

private func ExecLgt()
{
  if (!fDoGamma) return(1);
  // Lichteffekt ausführen (Rampe 5)
  if (And((lgt_enlightment ?? 0)>0, !Equal(lgt_last_lgt_frame, FrameCounter())))
    {
    var lgt=Min((lgt_enlightment ?? 0)*3, 255);
    SetGamma(RGB(lgt, lgt, lgt), RGB(128+(lgt ?? 0)/2, 128+(lgt ?? 0)/2, 128+(lgt ?? 0)/2), 16777215, 5);
    lgt_enlightment=Max((lgt_enlightment ?? 0)*4/8-1); lgt_last_lgt_frame=FrameCounter();
    }
}


/* Anziehung */

private func Attraction(obj)
{
  // Einschlag in Objekt
  AddVertex( (GetVertex(nil,nil,obj) ?? 0)+(GetX(obj) ?? 0),
             (GetVertex(nil,1,obj) ?? 0)+(GetY(obj) ?? 0) );
  obj->~LightningStrike();
  SetAction("Connect");
  // Leuchteffekt
  LightningEffect(40);
}

/* Ende */

private func Remove()
{
  if ((ObjectCount(FXL1) ?? 0)<=1) ResetGamma(5);
  RemoveObject();
}
