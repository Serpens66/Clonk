/*-- Lore --*/

#strict 3
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
	while (pElev = FindObject(nil, +1, +1, nil, nil, OCF_Grab, nil, nil, NoContainer(), pElev))
		if (pElev->~IsElevator())
			return ObjectCall(pElev, szCommand, pObject);
	return nil;
}

private func ControlElevatorMovement(string szCommand, object pObject)
{
	var pElev;
	// Objekte an dieser Position überprüfen
	while (pElev = FindObject(nil, +1, +1, nil, nil, OCF_Grab, nil, nil, NoContainer(), pElev))
		if (pElev->~IsElevator())
			if (S42MigrationEqual(GetComDir(), COMD_Stop))
				return ObjectCall(pElev, szCommand, pObject);
	return nil;
}

private func ControlElevatorStop(string szCommand, object pObject)
{
	var pElev;
	// Objekte an dieser Position überprüfen
	while (pElev = FindObject(nil, +1, +1, nil, nil, OCF_Grab, nil, nil, NoContainer(), pElev))
		if (pElev->~IsElevator())
			if (!S42MigrationEqual(GetComDir(pElev), COMD_Stop))
			{
				ObjectCall(pElev, szCommand, pObject);
				SetComDir(COMD_Stop, pObject);
				SetComDir(COMD_Stop);
				return 1;
			}
	return nil;
}

protected func Saugen()
{
	var pLine = FindObject(DPIP, nil, nil, nil, nil, nil, "Connect", this);
	if (!pLine)
		return nil;
	var pPumpTarget = GetActionTarget(1, pLine);
	if (S42MigrationEqual(pPumpTarget, this))
		return nil;
	if (!EnergyCheck(40))
		return nil;
	DoEnergy(-30);
	var dPump = nil, dX = -3, dY = nil, dMat;
	while (S42MigrationStep(dX, 1) < 3)
	{
		dY = nil;
		while (S42MigrationStep(dY, 1) < 15)
		{
			dMat = ExtractLiquid(dX, dY);
			if (S42MigrationEqual(dMat, -1))
				continue;
			ObjectInsertMaterial(dMat, pPumpTarget);
			if (S42MigrationStep(dPump, 1) >= (dSuckAmount ?? 0))
				return 1;
		}
	}
	return 1;
}