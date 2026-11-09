/*-- EMP Granate --*/

#strict

// Ist eine Granate
#include GREN

func HitObject()
{
    CreateEMPWave();
    RemoveObject();
}

func CreateEMPWave()
{
    Sound("EMP");
    CreateObject(SHCK, 0, 0, GetOwner())->SetMaxSize(iDamage);
}

func ExplodeDelay() { if(explodedelay) {return(explodedelay);}; return(50); }

func IsExplosiv() {return(0);}