function LoadE11Effects()
{
	local i = Effect.Register("blaster/muzzle_flash");
	E11.SetMuzzleEffect(Effect.Register("blaster/muzzle_flash"));
	E11.SetSelectSound(RegisterSound("sound/weapons/blaster/select.wav"));
	E11.SetFlashSound(RegisterSound("sound/weapons/blaster/fire.wav"));
	E11.SetFiringSound(NULL_SOUND);
	E11.SetChargeSound(NULL_SOUND);
	E11.SetMissileSound(NULL_SOUND);
	E11.SetMissileHitSound(NULL_SOUND);
	E11.SetAltFlashSound(RegisterSound("sound/weapons/blaster/alt_fire.wav"));
	E11.SetAltFiringSound(NULL_SOUND);
	E11.SetAltChargeSound(NULL_SOUND);
	E11.SetAltMuzzleEffect(Effect.Register("blaster/muzzle_flash"));
	E11.SetAltMissileSound(NULL_SOUND);
	E11.SetWallImpactEffect(Effect.Register("blaster/wall_impact"));
	E11.SetFleshImpactEffect(Effect.Register("blaster/flesh_impact"));
	E11.SetDroidImpactEffect(Effect.Register("blaster/droid_impact"));
	E11.SetShotEffect(Effect.Register("blaster/shot"));
	Effect.Register("blaster/deflect");//Yea...this was in original init code, so its here too
	E11.SetAltMissileHitSound(NULL_SOUND);
	E11.SetAltMissileSound(NULL_SOUND);
	E11.SetMissileModel(NULL_MODEL);
	E11.SetAltMissileModel(NULL_MODEL);
	E11.SetMissileDLight(0.0);
	E11.SetAltMissileDLight(0.0);
}
