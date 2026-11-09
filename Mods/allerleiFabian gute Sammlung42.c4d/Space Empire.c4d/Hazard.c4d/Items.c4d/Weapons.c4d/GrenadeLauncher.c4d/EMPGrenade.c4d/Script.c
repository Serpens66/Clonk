/*-- EMP Granate --*/

#strict 3

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
    CreateObject(SHCK, nil, nil, GetOwner())->SetMaxSize(iDamage);
}

func ExplodeDelay() { if(explodedelay) {return(explodedelay);}; return(50); }

func IsExplosiv() {return(nil);}