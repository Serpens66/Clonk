/*-- Realistischere Treffergeschwindigkeit --*/

#strict 3
#appendto *


/* Getting Hit reduces object speed */

func CatchBlow(int iLevel, object pObj) {
//nur wenn Regel aktiv
iLevel ??= 0;
if(FindObject(HTMA)){
	if ((GetCategory(pObj) ?? 0) & (C4D_Object ?? 0) && (GetOCF(pObj) ?? 0) & (OCF_Collectible ?? 0) && (ObjectDistance(pObj) ?? 0)*10 < 8*(Sqrt((GetDefWidth(GetID()) ?? 0)*(GetDefHeight(GetID()) ?? 0)) ?? 0) ) {
		pObj->~Hit();
		// Reduces object speed by 90%
		if (pObj) {
			pObj->SetXDir((GetXDir() ?? 0)*5, nil, 100);
			pObj->SetYDir((GetYDir() ?? 0)*5, nil, 100);
		}
	}
                    }
	return _inherited(iLevel, pObj);
}