models/players/sentry/sentry
{

    {
        map models/players/sentry/red_light
        blendFunc GL_ONE GL_ONE
        glow
        rgbGen wave sin 0 1 1 5
    }
    {
        map textures/common/environ1
        blendFunc GL_DST_COLOR GL_SRC_COLOR
        detail
        tcGen environment
    }
}

models/players/stormpilot/helmet
{
    {
        map models/players/stormpilot/helmet
        glow
    }
    {
        map gfx/effects/chr_white_add_mild
        blendFunc GL_SRC_ALPHA GL_ONE
        tcGen environment
    }
    {
        map models/players/stormpilot/helmet_s
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/mark1/torso
{
    {
        map models/players/mark1/torso
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/mark1/torso_s
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}


models/players/stormtrooper/armor
{
	cull	twosided
    {
        map models/players/stormtrooper/armor
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/stormtrooper/armor_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/stormtrooper/helmet
{
    {
        map models/players/stormtrooper/helmet
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/stormtrooper/helmet_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/stormtrooper/torso_legs
{
    {
        map models/players/stormtrooper/torso_legs
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/stormtrooper/torso_legs_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/stormtrooper/armor_blue
{
    {
        map models/players/stormtrooper/armor_blue
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/stormtrooper/armor_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/stormtrooper/armor_red
{
    {
        map models/players/stormtrooper/armor_red
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/stormtrooper/armor_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/stormtrooper/helmet_blue
{
    {
        map models/players/stormtrooper/helmet_blue
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/stormtrooper/helmet_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/stormtrooper/helmet_red
{
    {
        map models/players/stormtrooper/helmet_red
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/stormtrooper/helmet_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/snowtrooper/snowtrooper_cape
{
	cull	twosided
    {
        map models/players/snowtrooper/snowtrooper_cape
        glow
        rgbGen lightingDiffuse
    }
}

models/players/snowtrooper/snowtrooper_armor_blue
{
    {
        map models/players/snowtrooper/snowtrooper_armor_blue
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/snowtrooper/snowtrooper_armor_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/snowtrooper/snowtrooper_armor_red
{
    {
        map models/players/snowtrooper/snowtrooper_armor_red
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/snowtrooper/snowtrooper_armor_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/snowtrooper/snowtrooper_cape_blue
{
	cull	twosided
    {
        map models/players/snowtrooper/snowtrooper_cape_blue
        glow
        rgbGen lightingDiffuse
    }
}

models/players/snowtrooper/snowtrooper_cape_red
{
	cull	twosided
    {
        map models/players/snowtrooper/snowtrooper_cape_red
        glow
        rgbGen lightingDiffuse
    }
}

models/players/snowtrooper/snowtrooper_torso
{
    {
        map models/players/snowtrooper/snowtrooper_torso
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/snowtrooper/snowtrooper_torso_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/snowtrooper/snowtrooper_torso_blue
{
    {
        map models/players/snowtrooper/snowtrooper_torso_blue
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/snowtrooper/snowtrooper_torso_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/snowtrooper/snowtrooper_armor
{
    {
        map models/players/snowtrooper/snowtrooper_armor
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/snowtrooper/snowtrooper_armor_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/jedi_tf/torso_03_arms
{
    {
        map models/players/jedi_tf/torso_03_arms
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
}

models/players/jedi_tf/torso_03_clothes
{
    {
        map models/players/jedi_tf/torso_03
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/torso_03_skin
{
    {
        map models/players/jedi_tf/torso_03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
}

models/players/jedi_tf/face
{
    {
        map models/players/jedi_tf/face
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_tf/face
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/head
{
    {
        map models/players/jedi_tf/head
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
}

models/players/jedi_tf/tentacles
{
    {
        map models/players/jedi_tf/tentacles
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
}

models/players/jedi_tf/tentacles_01
{
    {
        map models/players/jedi_tf/tentacles_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_tf/tentacles_01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/face_01
{
    {
        map models/players/jedi_tf/face_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_tf/face_01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/head_01
{
    {
        map models/players/jedi_tf/head_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_tf/head_01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/head_03
{
    {
        map models/players/jedi_tf/head_03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_tf/head_03
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/torso_01_clothes
{
    {
        map models/players/jedi_tf/torso_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/torso_01_skin
{
    {
        map models/players/jedi_tf/torso_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
}

models/players/jedi_tf/torso_01_lower
{
	cull	twosided
    {
        map models/players/jedi_tf/torso_01_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/torso_02_arms
{
    {
        map models/players/jedi_tf/torso_02_arms
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
}

models/players/jedi_tf/torso_02_clothes
{
    {
        map models/players/jedi_tf/torso_02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/torso_02_skin
{
    {
        map models/players/jedi_tf/torso_02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_tf/torso_02
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/torso_06_arms
{
    {
        map models/players/jedi_tf/torso_06_arms
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
}

models/players/jedi_tf/torso_06_clothes
{
    {
        map models/players/jedi_tf/torso_06
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/torso_06_skin
{
    {
        map models/players/jedi_tf/torso_06
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
}

models/players/jedi_tf/hips_02_clothes
{
    {
        map models/players/jedi_tf/hips_02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/hips_02_skin
{
    {
        map models/players/jedi_tf/hips_02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
}

models/players/jedi_tf/torso_02_lower
{
	cull	twosided
    {
        map models/players/jedi_tf/torso_02_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/icon_torso_f1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_torso_f1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_torso_f1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_head_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_head_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_head_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_head_a2
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_head_a2
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_head_a2
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_head_a3
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_head_a3
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_head_a3
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_head_a4
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_head_a4
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_head_a4
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_head_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_head_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_head_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_head_b2
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_head_b2
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_head_b2
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_head_b3
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_head_b3
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_head_b3
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_head_b4
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_head_b4
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_head_b4
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_lower_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_lower_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_lower_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_torso_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_torso_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_torso_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_torso_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_torso_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_torso_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_torso_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_torso_c1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_torso_c1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/torso_04
{
    {
        map models/players/jedi_tf/torso_04
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_tf/torso_04
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/torso_05
{
    {
        map models/players/jedi_tf/torso_05
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_tf/torso_05
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/hips_01
{
    {
        map models/players/jedi_tf/hips_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_tf/hips_01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_tf/icon_torso_d1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_torso_d1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_torso_d1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_torso_e1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_torso_e1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_torso_e1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_lower_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_tf/icon_lower_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_tf/icon_lower_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_tf/icon_lower_c1
{
	nopicmip
	nomipmaps
	q3map_nolightmap
    {
        map models/players/jedi_tf/icon_lower_c1
        glow
        rgbGen identity
    }
}

models/players/jedi_tf/icon_lower_d1
{
	nopicmip
	nomipmaps
	q3map_nolightmap
    {
        map models/players/jedi_tf/icon_lower_d1
        glow
    }
}

models/players/rancor/rancor_body
{
    {
        map models/players/rancor/rancor_body
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/rancor/rancor_body_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/rancor/rancor_teeth
{
    {
        map models/players/rancor/rancor_head
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/rancor/rancor_head_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/chewbacca/chewbacca_2sided
{
	cull	twosided
    {
        map models/players/chewbacca/chewbacca_2sided
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        glow
        rgbGen lightingDiffuse
    }
}

models/players/chewbacca/chewbacca_1sided
{
	cull	twosided
    {
        map models/players/chewbacca/chewbacca_1sided
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        glow
        rgbGen lightingDiffuse
    }
}

models/players/rodian/tint_torso
{
    {
        map models/players/rodian/tint_torso
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/rodian/tint_torso
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}
models/players/rodian/tint_boots_belt_vest
{
    {
        map models/players/rodian/tint_boots_belt_vest
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/rodian/tint_boots_belt_vest
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/rodian/tint_l_boot
{
    {
        map models/players/rodian/tint_l_boot
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/rodian/tint_l_boot
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/head03_alpha
{
	cull	twosided
    {
        map models/players/jedi_rm/head03_alpha
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/head02_alpha
{
	cull	twosided
    {
        map models/players/jedi_rm/head02_alpha
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/torso_06_lower
{
	cull	twosided
    {
        map models/players/jedi_rm/torso_06_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/torso_06_lower
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/arms_01
{
    {
        map models/players/jedi_rm/arms_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/arms_01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/arms_03_clothes
{
    {
        map models/players/jedi_rm/arms_03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/arms_03
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/torso_01
{
    {
        map models/players/jedi_rm/torso_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/torso_01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/torso_03
{
    {
        map models/players/jedi_rm/torso_03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/torso_03
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/torso_03_lower
{
	cull	twosided
    {
        map models/players/jedi_rm/torso_03_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/torso_03_lower
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/torso_05
{
    {
        map models/players/jedi_rm/torso_05
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/torso_05
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/torso_06
{
    {
        map models/players/jedi_rm/torso_06
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/torso_06
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/icon_lower_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_lower_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_rm/icon_lower_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_rm/icon_lower_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_lower_c1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_rm/icon_lower_c1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_rm/icon_torso_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_torso_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_rm/icon_torso_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_rm/icon_torso_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_torso_c1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_rm/icon_torso_c1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_rm/icon_torso_d1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_torso_d1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_rm/icon_torso_d1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_rm/icon_torso_e1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_torso_e1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_rm/icon_torso_e1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_rm/icon_torso_f1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_torso_f1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_rm/icon_torso_f1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_rm/torso_04_lower
{
	cull	twosided
    {
        map models/players/jedi_rm/torso_04_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/torso_04_lower
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/torso_04
{
    {
        map models/players/jedi_rm/torso_04
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/torso_04
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/arms_04
{
    {
        map models/players/jedi_rm/arms_04
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/arms_04
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/torso_02
{
    {
        map models/players/jedi_rm/torso_02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/torso_02
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/arms_02
{
    {
        map models/players/jedi_rm/arms_02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/arms_02
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/icon_lower_d1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_lower_d1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_rm/icon_lower_d1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_rm/pants01
{
	cull	twosided
    {
        map models/players/jedi_rm/pants01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/pants01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/pants02
{
	cull	twosided
    {
        map models/players/jedi_rm/pants02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/pants02
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/pants03
{
	cull	twosided
    {
        map models/players/jedi_rm/pants03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/pants03
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/pants04
{
	cull	twosided
    {
        map models/players/jedi_rm/pants04
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/pants04
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/icon_torso_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_torso_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_rm/icon_torso_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_rm/arms_05_clothes
{
    {
        map models/players/jedi_rm/arms_05
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_rm/arms_05
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_rm/icon_lower_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_lower_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_rm/icon_lower_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_rm/icon_head_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_head_a1
        glow
        rgbGen identity
    }
}

models/players/jedi_rm/icon_head_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_head_b1
        glow
        rgbGen identity
    }
}

models/players/jedi_rm/icon_head_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_head_c1
        glow
        rgbGen identity
    }
}

models/players/rockettrooper/rockettrooper_torso
{
    {
        map models/players/rockettrooper/rockettrooper_torso
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/rockettrooper/rockettrooper_torso_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/rockettrooper/rockettrooper_rocket
{
    {
        map models/players/rockettrooper/rockettrooper_rocket
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/rockettrooper/rockettrooper_rocket_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/rockettrooper/rockettrooper_torso_red
{
    {
        map models/players/rockettrooper/rockettrooper_torso_red
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/rockettrooper/rockettrooper_torso_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/rockettrooper/rockettrooper_arms_red
{
    {
        map models/players/rockettrooper/rockettrooper_arms_red
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/rockettrooper/rockettrooper_arms_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/rockettrooper/rockettrooper_torso_blue
{
    {
        map models/players/rockettrooper/rockettrooper_torso_blue
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/rockettrooper/rockettrooper_torso_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/jedi_hm/robes01
{
    {
        map models/players/jedi_hm/robes01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/robes01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/robes01_arms
{
    {
        map models/players/jedi_hm/robes01_arms
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/robes01_lower
{
	cull	twosided
    {
        map models/players/jedi_hm/robes01_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/robes01_lower
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/robes02
{
    {
        map models/players/jedi_hm/robes02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/robes02
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/robes02_arms
{
    {
        map models/players/jedi_hm/robes02_arms
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/robes02_arms
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/robes02_lower
{
	cull	twosided
    {
        map models/players/jedi_hm/robes02_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/robes02_lower
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/teeth
{
	cull	twosided
    {
        map models/players/jedi_hm/teeth
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/vest01
{
    {
        map models/players/jedi_hm/vest01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/vest01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/vest01_arms
{
    {
        map models/players/jedi_hm/vest01_arms
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/vest01_arms
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/vest01b
{
    {
        map models/players/jedi_hm/vest01b
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/vest01b
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/vest01b_arms
{
    {
        map models/players/jedi_hm/vest01b_arms
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/vest01b_arms
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/vest03
{
    {
        map models/players/jedi_hm/vest03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/vest03
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/vest03_arms_clothes
{
    {
        map models/players/jedi_hm/vest03_arms
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/hair02_head_bang
{
	cull	twosided
    {
        map models/players/jedi_hm/hair02_head_bang
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/icon_lower_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_lower_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hm/icon_lower_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/icon_lower_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_lower_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hm/icon_lower_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/icon_torso_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_torso_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hm/icon_torso_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/icon_torso_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_torso_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hm/icon_torso_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/icon_torso_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_torso_c1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hm/icon_torso_c1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/icon_torso_d1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_torso_d1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hm/icon_torso_d1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/icon_torso_d2
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_torso_d2
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hm/icon_torso_d2
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/icon_torso_f1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_torso_f1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hm/icon_torso_f1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/robes03_arms
{
    {
        map models/players/jedi_hm/robes03_arms
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/robes03_arms
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/robes03
{
    {
        map models/players/jedi_hm/robes03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/robes03
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/pants01
{
    {
        map models/players/jedi_hm/pants01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/pants01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/pants02
{
    {
        map models/players/jedi_hm/pants02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/pants02
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/pants03
{
    {
        map models/players/jedi_hm/pants03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/pants03
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/pants04
{
    {
        map models/players/jedi_hm/pants04
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/pants04
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/icon_torso_e1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_torso_e1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hm/icon_torso_e1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/icon_lower_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_lower_c1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hm/icon_lower_c1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/icon_lower_d1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_lower_d1
        glow
    }
    {
        map models/players/jedi_hm/icon_lower_d1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/vest02
{
    {
        map models/players/jedi_hm/vest02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/vest02
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/vest02_arms
{
    {
        map models/players/jedi_hm/vest02_arms
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hm/vest02_arms
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hm/icon_head_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_head_a1
        glow
        rgbGen identity
    }
}

models/players/jedi_hm/icon_head_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_head_b1
        glow
        rgbGen identity
    }
}

models/players/jedi_hm/icon_head_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hm/icon_head_c1
        glow
        rgbGen identity
    }
}

models/players/jedi_hf/teeth
{
	cull	twosided
    {
        map models/players/jedi_hf/teeth
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/torso_01
{
    {
        map models/players/jedi_hf/torso_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/torso_01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/torso_01_lower
{
	cull	twosided
    {
        map models/players/jedi_hf/torso_01_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/torso_02
{
    {
        map models/players/jedi_hf/torso_02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/torso_02
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/torso_02_lower
{
	cull	twosided
    {
        map models/players/jedi_hf/torso_02_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/torso_03_clothes
{
    {
        map models/players/jedi_hf/torso_03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/torso_03
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/torso_04
{
    {
        map models/players/jedi_hf/torso_04
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/torso_04
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/arms_01
{
    {
        map models/players/jedi_hf/arms_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/arms_01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
    }
}

models/players/jedi_hf/arms_04
{
    {
        map models/players/jedi_hf/arms_04
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/arms_04
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/hair_03_alpha
{
	cull	twosided
    {
        map models/players/jedi_hf/hair_03_alpha
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/hair_02_alpha
{
	cull	twosided
    {
        map models/players/jedi_hf/hair_02_alpha
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        depthWrite
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/icon_torso_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_torso_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hf/icon_torso_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hf/icon_torso_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_torso_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hf/icon_torso_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hf/icon_torso_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_torso_c1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hf/icon_torso_c1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hf/icon_torso_d1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_torso_d1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hf/icon_torso_d1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hf/icon_lower_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_lower_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hf/icon_lower_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hf/icon_lower_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_lower_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hf/icon_lower_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hf/torso_05
{
    {
        map models/players/jedi_hf/torso_05
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/torso_05
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/torso_06
{
    {
        map models/players/jedi_hf/torso_06
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/torso_06
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/torso_06_lower
{
	cull	twosided
    {
        map models/players/jedi_hf/torso_06_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/torso_06_lower
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/arms_05
{
    {
        map models/players/jedi_hf/arms_05
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/arms_05
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/pants_01
{
    {
        map models/players/jedi_hf/pants_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/pants_01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/pants_02
{
    {
        map models/players/jedi_hf/pants_02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/pants_02
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/pants_03
{
    {
        map models/players/jedi_hf/pants_03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/pants_03
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/pants_04
{
    {
        map models/players/jedi_hf/pants_04
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hf/pants_04
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hf/icon_torso_e1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_torso_e1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hf/icon_torso_e1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hf/icon_torso_f1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_torso_f1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hf/icon_torso_f1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hf/icon_lower_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_lower_c1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hf/icon_lower_c1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hf/icon_lower_d1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_lower_d1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_hf/icon_lower_d1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hf/icon_head_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_head_a1
        glow
        rgbGen identity
    }
}

models/players/jedi_hf/icon_head_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_head_b1
        glow
        rgbGen identity
    }
}

models/players/jedi_hf/icon_head_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hf/icon_head_c1
        glow
        rgbGen identity
    }
}

models/players/jedi_zf/torso_03_clothes
{
    {
        map models/players/jedi_zf/torso_03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/torso_03
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/torso_03_lower
{
	cull	twosided
    {
        map models/players/jedi_zf/torso_03_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/torso_03_lower
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/torso_04_clothes
{
    {
        map models/players/jedi_zf/torso_04
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/torso_04
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/torso_05_clothes
{
    {
        map models/players/jedi_zf/torso_05
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/torso_05
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/torso_05_lower
{
	cull	twosided
    {
        map models/players/jedi_zf/torso_05_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/torso_06_clothes
{
    {
        map models/players/jedi_zf/torso_06
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/torso_06
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/torso_06_lower
{
	cull	twosided
    {
        map models/players/jedi_zf/torso_06_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/torso_06_lower
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/arms_06_clothes
{
    {
        map models/players/jedi_zf/arms_06
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/arms_06
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/hands_04_wrist_armor
{
    {
        map models/players/jedi_zf/hands_04_wrist_armor
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/hands_04_wrist_armor
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/icon_torso_e1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_torso_e1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_zf/icon_torso_e1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_zf/icon_torso_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_torso_c1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_zf/icon_torso_c1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_zf/icon_torso_d1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_torso_d1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_zf/icon_torso_d1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_zf/icon_torso_f1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_torso_f1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_zf/icon_torso_f1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_zf/icon_lower_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_lower_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_zf/icon_lower_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_zf/icon_lower_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_lower_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_zf/icon_lower_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_zf/torso_01
{
    {
        map models/players/jedi_zf/torso_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/torso_01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/torso_01_lower
{
	cull	twosided
    {
        map models/players/jedi_zf/torso_01_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/torso_01_lower
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/torso_02_lower
{
	cull	twosided
    {
        map models/players/jedi_zf/torso_02_lower
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/torso_02_lower
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/torso_02
{
    {
        map models/players/jedi_zf/torso_02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/torso_02
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/hips_02
{
    {
        map models/players/jedi_zf/hips_02
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/hips_02
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/hips_01
{
    {
        map models/players/jedi_zf/hips_01
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/hips_01
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/hips_03
{
    {
        map models/players/jedi_zf/hips_03
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/hips_03
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/hips_04
{
    {
        map models/players/jedi_zf/hips_04
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_zf/hips_04
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/jedi_zf/icon_torso_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_torso_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_zf/icon_torso_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_zf/icon_torso_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_torso_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_zf/icon_torso_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_zf/icon_lower_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_lower_c1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_zf/icon_lower_c1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_zf/icon_lower_d1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_lower_d1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_zf/icon_lower_d1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_zf/icon_head_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_head_a1
        glow
        rgbGen identity
    }
}

models/players/jedi_zf/icon_head_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_head_b1
        glow
        rgbGen identity
    }
}

models/players/jedi_zf/icon_head_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_zf/icon_head_c1
        glow
        rgbGen identity
    }
}

models/players/saboteur/saboteur_face_plate
{
    {
        map textures/colors/black
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        glow
        rgbGen lightingDiffuse
        alphaGen const 0.72
    }
    {
        map textures/common/env_chrome
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        alphaGen const 0.13
        tcGen environment
    }
}

models/players/tauntaun/tauntaun_teeth_strap
{
	cull	twosided
    {
        map models/players/tauntaun/tauntaun_teeth_strap
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        glow
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/torso_5
{
    {
        map models/players/jedi_kdm/torso_5
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/torso_5
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/torso_2
{
    {
        map models/players/jedi_kdm/torso_2
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/torso_2
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/torso_3
{
    {
        map models/players/jedi_kdm/torso_3
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/torso_3
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/torso_4
{
    {
        map models/players/jedi_kdm/torso_4
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/torso_4
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/icon_lower_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_lower_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_kdm/icon_lower_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_kdm/icon_lower_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_lower_c1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_kdm/icon_lower_c1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_kdm/icon_torso_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_torso_a1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_kdm/icon_torso_a1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_kdm/icon_torso_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_torso_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_kdm/icon_torso_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_kdm/icon_torso_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_torso_c1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_kdm/icon_torso_c1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_kdm/icon_torso_d1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_torso_d1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_kdm/icon_torso_d1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_kdm/icon_torso_e1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_torso_e1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_kdm/icon_torso_e1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_kdm/torso_1
{
    {
        map models/players/jedi_kdm/torso_1
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/torso_1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/arms_1
{
    {
        map models/players/jedi_kdm/arms_1
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/arms_1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/robe_1
{
	cull	twosided
    {
        map models/players/jedi_kdm/robe_1
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/robe_1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/arms_2
{
    {
        map models/players/jedi_kdm/arms_2
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/arms_2
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/arms_3
{
    {
        map models/players/jedi_kdm/arms_3
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/arms_3
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/robe_3
{
	cull	twosided
    {
        map models/players/jedi_kdm/robe_3
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/robe_3
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/arms_4
{
    {
        map models/players/jedi_kdm/arms_4
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/arms_4
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen identityLighting
    }
}

models/players/jedi_kdm/torso_6
{
    {
        map models/players/jedi_kdm/torso_6
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/torso_6
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/arms_6
{
    {
        map models/players/jedi_kdm/arms_6
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/arms_6
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/icon_torso_f1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_torso_f1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_kdm/icon_torso_f1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_kdm/robe_5
{
	cull	twosided
    {
        map models/players/jedi_kdm/robe_5
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/robe_5
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/icon_lower_d1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_lower_d1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_kdm/icon_lower_d1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_kdm/pants_1
{
    {
        map models/players/jedi_kdm/pants_1
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/pants_1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/pants_4
{
    {
        map models/players/jedi_kdm/pants_4
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/pants_4
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/pants_2
{
    {
        map models/players/jedi_kdm/pants_2
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/pants_2
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/pants_3
{
    {
        map models/players/jedi_kdm/pants_3
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_kdm/pants_3
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_kdm/icon_lower_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_lower_b1
        glow
        rgbGen identity
    }
    {
        map models/players/jedi_kdm/icon_lower_b1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_kdm/icon_head_a1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_head_a1
        glow
        rgbGen identity
    }
}

models/players/jedi_kdm/icon_head_b1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_head_b1
        glow
        rgbGen identity
    }
}

models/players/jedi_kdm/icon_head_c1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_head_c1
        glow
        rgbGen identity
    }
}

models/players/jawa/jawa_eyes
{
    {
        map models/players/jawa/jawa_eyes
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/jawa/jawa_eyes
        blendFunc GL_ONE GL_ONE
        glow
        rgbGen identity
    }
}

models/players/x-wing/xwings
{
    {
        map models/players/x-wing/xwings
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/x-wing/xwings_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/x-wing/xwglass_shd
{
    {
        map textures/colors/black
        glow
        rgbGen lightingDiffuse
    }
    {
        map textures/common/env_chrome
        blendFunc GL_ONE_MINUS_SRC_ALPHA GL_SRC_ALPHA
        alphaGen const 0.9
        tcGen environment
    }
    {
        map models/players/x-wing/xwglass_shd_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        alphaGen lightingSpecular
    }
    {
        map models/players/x-wing/xwglass_shd
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/x-wing/xwbody
{
    {
        map models/players/x-wing/xwbody
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/x-wing/xwbody_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/marka_ragnos/marka_robe
{
    {
        map models/players/marka_ragnos/marka_robe
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        depthWrite
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/marka_ragnos/marka_robe_glow2
        blendFunc GL_ONE GL_ONE
        glow
    }
}

models/players/marka_ragnos/marka_arms
{
    {
        map models/players/marka_ragnos/marka_arms
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/marka_ragnos/marka_arms_glow2
        blendFunc GL_ONE GL_ONE
        glow
    }
}

models/players/marka_ragnos/marka_head
{
    {
        map models/players/marka_ragnos/marka_head
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/marka_ragnos/marka_head_glow2
        blendFunc GL_ONE GL_ONE
        glow
    }
}

models/players/marka_ragnos/marka_torso
{
    {
        map models/players/marka_ragnos/marka_torso
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/marka_ragnos/marka_torso_glow2
        blendFunc GL_ONE GL_ONE
        glow
    }
}

models/players/tavion_new/torso_blue_glow
{
    {
        map models/players/tavion_new/torso_blue
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/tavion_new/torso_blue_glow2
        blendFunc GL_ONE GL_ONE
        glow
    }
}

models/players/tavion_new/head_glow
{
    {
        map models/players/tavion_new/head
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/tavion_new/head_glow2
        blendFunc GL_ONE GL_ONE
        glow
    }
}

models/players/tavion_new/feathers_glow
{
    {
        map models/players/tavion_new/feathers
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/tavion_new/feathers_glow2
        blendFunc GL_ONE GL_ONE
        glow
    }
}

models/players/tavion_new/face_glow
{
    {
        map models/players/tavion_new/face
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/tavion_new/face_glow2
        blendFunc GL_ONE GL_ONE
        glow
    }
}

models/players/tavion_new/arm_blue_glow
{
    {
        map models/players/tavion_new/arm_blue
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/tavion_new/arm_blue_glow2
        blendFunc GL_ONE GL_ONE
        glow
    }
}

models/players/tavion_new/hands_glow
{
    {
        map models/players/tavion_new/hands
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/tavion_new/hands_glow2
        blendFunc GL_ONE GL_ONE
        glow
    }
}

models/players/tavion_new/legs_blue_glow
{
    {
        map models/players/tavion_new/legs_blue
        blendFunc GL_ONE GL_ZERO
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/tavion_new/legs_blue_glow2
        blendFunc GL_ONE GL_ONE
        glow
    }
}

models/players/lambdashuttle/shuttle_wing02
{
    {
        map models/players/lambdashuttle/shuttle_wing02
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/lambdashuttle/wing_left_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/lambdashuttle/shuttle_nose
{
    {
        map models/players/lambdashuttle/shuttle_nose
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/lambdashuttle/shuttle_nose_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/lambdashuttle/shuttle_wing01
{
    {
        map models/players/lambdashuttle/shuttle_wing01
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/lambdashuttle/wing_center_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/lambdashuttle/shuttle_body
{
    {
        map models/players/lambdashuttle/shuttle_body
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/lambdashuttle/shuttle_body_glow
        blendFunc GL_ONE GL_ONE
        glow
    }
    {
        map models/players/lambdashuttle/shutttle_body_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/boba_fett/boba_larmalpha
{
    {
        map models/players/boba_fett/boba_larmalpha
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        glow
        rgbGen lightingDiffuse
    }
}

models/players/boba_fett/boba_alpha
{
    {
        map models/players/boba_fett/boba_alpha
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        glow
        rgbGen lightingDiffuse
    }
}

models/players/boba_fett/boba_body
{
    {
        map models/players/boba_fett/boba_body
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/boba_fett/boba_body_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/boba_fett/boba_hands
{
    {
        map models/players/boba_fett/boba_hands
        glow
        rgbGen lightingDiffuse
    }
}

models/players/boba_fett/boba_head_revised
{
    {
        map models/players/boba_fett/boba_head_revised
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/boba_fett/boba_head_revised_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/boba_fett/boba_jetpack
{
    {
        map models/players/boba_fett/boba_jetpack
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/boba_fett/boba_jetpack_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/boba_fett/boba_head
{
    {
        map models/players/boba_fett/boba_head_revised
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/boba_fett/boba_head_revised_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/boba_fett/boba_body_red
{
    {
        map models/players/boba_fett/boba_body_red
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/boba_fett/boba_body_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/boba_fett/boba_body_blue
{
    {
        map models/players/boba_fett/boba_body_blue
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/boba_fett/boba_body_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/boba_fett/boba_head_revised_red
{
    {
        map models/players/boba_fett/boba_head_revised_red
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/boba_fett/boba_head_revised_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/boba_fett/boba_head_revised_blue
{
    {
        map models/players/boba_fett/boba_head_revised_blue
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/boba_fett/boba_head_revised_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/boba_fett/boba_jetpak_red
{
    {
        map models/players/boba_fett/boba_jetpack_red
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/boba_fett/boba_jetpack_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/boba_fett/boba_jetpak_blue
{
    {
        map models/players/boba_fett/boba_jetpack_blue
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/boba_fett/boba_jetpack_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/hazardtrooper/hazardtrooper_torso_red
{
    {
        map models/players/hazardtrooper/hazardtrooper_torso_red
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/hazardtrooper/hazardtrooper_torso_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/hazardtrooper/hazardtrooper_arms_blue
{
    {
        map models/players/hazardtrooper/hazardtrooper_arms_blue
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/hazardtrooper/hazardtrooper_arms_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/hazardtrooper/hazardtrooper_arms_red
{
    {
        map models/players/hazardtrooper/hazardtrooper_arms_red
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/hazardtrooper/hazardtrooper_arms_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/hazardtrooper/hazardtrooper_legs
{
    {
        map models/players/hazardtrooper/hazardtrooper_legs
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/hazardtrooper/hazardtrooper_legs_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/hazardtrooper/hazardtrooper_legs_blue
{
    {
        map models/players/hazardtrooper/hazardtrooper_legs_blue
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/hazardtrooper/hazardtrooper_legs_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/hazardtrooper/hazardtrooper_legs_red
{
    {
        map models/players/hazardtrooper/hazardtrooper_legs_red
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/hazardtrooper/hazardtrooper_legs_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/hazardtrooper/hazardtrooper_torso
{
    {
        map models/players/hazardtrooper/hazardtrooper_torso
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/hazardtrooper/hazardtrooper_torso_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/hazardtrooper/hazardtrooper_torso_blue
{
    {
        map models/players/hazardtrooper/hazardtrooper_torso_blue
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/hazardtrooper/hazardtrooper_torso_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/hazardtrooper/hazardtrooper_arms
{
    {
        map models/players/hazardtrooper/hazardtrooper_arms
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/hazardtrooper/hazardtrooper_arms_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/howler/howler
{
    {
        map models/players/howler/howler
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/howler/howler_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/saber_droid/saber_droid_legs
{
    {
        map models/players/saber_droid/saber_droid_legs
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/saber_droid/saber_droid_legs_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/saber_droid/saber_droid_head
{
    {
        map models/players/saber_droid/saber_droid_head
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/saber_droid/saber_droid_head_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/saber_droid/saber_droid_arms
{
    {
        map models/players/saber_droid/saber_droid_arms
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/saber_droid/saber_droid_arms_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/r2d2/legs_r2d2
{
    {
        map models/players/r2d2/legs_r2d2
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/r2d2/legs_r2d2_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/r2d2/imp_body_r2d2
{
    {
        map models/players/r2d2/imp_body_r2d2
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/r2d2/imp_body_r2d2_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/r2d2/imp_legs_r2d2
{
    {
        map models/players/r2d2/imp_legs_r2d2
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/r2d2/imp_legs_r2d2_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/r2d2/body_r2d2
{
    {
        map models/players/r2d2/body_r2d2
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/r2d2/body_r2d2_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/remote/remote
{
    {
        map models/players/remote/remote
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/remote/remote_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/mutant_rancor/pistons2
{
    {
        map models/players/mutant_rancor/pistons2
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/mutant_rancor/pistons2_glow
        blendFunc GL_ONE GL_ONE
        glow
        detail
        rgbGen identity
    }
    {
        map models/players/mutant_rancor/pistons2_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/mutant_rancor/head2
{
    {
        map models/players/mutant_rancor/head2
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/mutant_rancor/head2_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/mutant_rancor/body2
{
    {
        map models/players/mutant_rancor/body2
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/mutant_rancor/body2_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/r5d2/legs_r5d2
{
    {
        map models/players/r5d2/legs_r5d2
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/r5d2/legs_r5d2_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/r5d2/body_r5d2
{
    {
        map models/players/r5d2/body_r5d2
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/r5d2/body_r5d2_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/weequay/tint_torso
{
    {
        map models/players/weequay/tint_torso
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/weequay/tint_torso
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/trandoshan/tint_trandoshan_torso
{
    {
        map models/players/trandoshan/tint_trandoshan_torso
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/trandoshan/tint_trandoshan_torso
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/trandoshan/tint_trandoshan_legs
{
    {
        map models/players/trandoshan/tint_trandoshan_legs
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/trandoshan/tint_trandoshan_legs
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/gran/tint_torso
{
    {
        map models/players/gran/tint_torso
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/gran/tint_torso
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/gran/tint_gran_legs
{
    {
        map models/players/gran/tint_gran_legs
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/gran/tint_gran_legs
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/gran/tint_r_arm
{
    {
        map models/players/gran/tint_r_arm
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/gran/tint_r_arm
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/gran/tint_l_arm
{
    {
        map models/players/gran/tint_l_arm
        glow
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/gran/tint_l_arm
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        detail
        rgbGen lightingDiffuse
    }
}

models/players/sand_creature/worm
{
    {
        map models/players/sand_creature/worm
        glow
        rgbGen lightingDiffuse
    }
    {
        map models/players/sand_creature/worm_spec
        blendFunc GL_SRC_ALPHA GL_ONE
        detail
        alphaGen lightingSpecular
    }
}

models/players/rebel_pilot/googles
{
    {
        map models/players/rebel_pilot/googles
        blendFunc GL_DST_COLOR GL_SRC_COLOR
        glow
        rgbGen lightingDiffuse
    }
}

models/players/reborn_new/head_hat
{
	qer_editorimage	models/players/reborn_new/head
	q3map_nolightmap
	cull	twosided
    {
        map models/players/reborn_new/head_hat
        glow
        rgbGen lightingDiffuse
    }
}

