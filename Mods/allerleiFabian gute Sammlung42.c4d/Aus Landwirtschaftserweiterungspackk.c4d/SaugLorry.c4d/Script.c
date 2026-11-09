/*-- Lore --*/

#strict 2
#include CXEC // Energieversorgung

local dSuckAmount;

public func MaxSuckAmount()
{
	return 3 + 1000;
}

/* Steuerung */
public func Initialize()
{
	dSuckAmount = MaxSuckAmount();
	return 1;
}

protected func ContactLeft()
{
	if (Stuck() && !Random(5))
		SetRDir(RandomX(-7, +7));
}

protected func ContactRight()
{
	if (Stuck() && !Random(5))
		SetRDir(RandomX(-7, +7));
}

private func ControlElevator(string szCommand, object pObject)
{
	var pElev;
	// Objekte an dieser Position überprüfen
	while (pElev = FindObject(0, +1, +1, 0, 0, OCF_Grab, 0, 0, NoContainer(), pElev))
		if (pElev->~IsElevator())
			return ObjectCall(pElev, szCommand, pObject);
	return 0;
}

private func ControlElevatorMovement(string szCommand, object pObject)
{
	var pElev;
	// Objekte an dieser Position überprüfen
	while (pElev = FindObject(0, +1, +1, 0, 0, OCF_Grab, 0, 0, NoContainer(), pElev))
		if (pElev->~IsElevator())
			if (GetComDir() == COMD_Stop)
				return ObjectCall(pElev, szCommand, pObject);
	return 0;
}

private func ControlElevatorStop(string szCommand, object pObject)
{
	var pElev;
	// Objekte an dieser Position überprüfen
	while (pElev = FindObject(0, +1, +1, 0, 0, OCF_Grab, 0, 0, NoContainer(), pElev))
		if (pElev->~IsElevator())
			if (GetComDir(pElev) != COMD_Stop)
			{
				ObjectCall(pElev, szCommand, pObject);
				SetComDir(COMD_Stop, pObject);
				SetComDir(COMD_Stop);
				return 1;
			}
	return 0;
}

protected func Saugen()
{
	var pLine = FindObject(DPIP, 0, 0, 0, 0, 0, "Connect", this);
	if (!pLine)
		return 0;
	var pPumpTarget = GetActionTarget(1, pLine);
	if (pPumpTarget == this)
		return 0;
	if (!EnergyCheck(40))
		return 0;
	DoEnergy(-30);
	var dPump = 0, dX = -3, dY = 0, dMat;
	while (++dX < 3)
	{
		dY = 0;
		while (++dY < 15)
		{
			dMat = ExtractLiquid(dX, dY);
			if (dMat == -1)
				continue;
			ObjectInsertMaterial(dMat, pPumpTarget);
			if (++dPump >= dSuckAmount)
				return 1;
		}
	}
	return 1;
}