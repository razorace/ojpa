//Player Model Icon Shaders

//Human Male Icons
//This is the type of shader used for the player menu icons that 
//have selectable color areas.
//These are stored in .png format (so they can have an alpha channel)

//For most of the player classes, I've shared the icons by placing dummy 
//.png icon files (so the menu tries to load them) in the player model 
//directories and then using shader referencing to use the general icons.  
//The generic icons are located in the jedi_hoth directory.  
		
//Please note that the jedi_kdm, jedi_rm, and jedi_tf have custom torso 
//icons and jedi_tf has a custom lower icon.  Those custom icons are found 
//in the individual model directories.

models/players/jedi_hm/icon_torso_h1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hoth/icon_torso_male
        rgbGen identity
    }
    {
        map models/players/jedi_hoth/icon_torso_male
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hm/icon_lower_f1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hoth/icon_lower_male
        rgbGen identity
    }
    {
        map models/players/jedi_hoth/icon_lower_male
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}


models/players/jedi_hf/icon_torso_h1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hoth/icon_torso_female
        rgbGen identity
    }
    {
        map models/players/jedi_hoth/icon_torso_female
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_hf/icon_lower_f1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hoth/icon_lower_female
        rgbGen identity
    }
    {
        map models/players/jedi_hoth/icon_lower_female
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}


//Zabrak Female Icons
models/players/jedi_zf/icon_torso_h1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hoth/icon_torso_female
        rgbGen identity
    }
    {
        map models/players/jedi_hoth/icon_torso_female
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_zf/icon_lower_f1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hoth/icon_lower_female
        rgbGen identity
    }
    {
        map models/players/jedi_hoth/icon_lower_female
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}


//Kel Dor Male Icons
models/players/jedi_kdm/icon_torso_h1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_kdm/icon_torso_h1
        rgbGen identity
    }
    {
        map models/players/jedi_kdm/icon_torso_h1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_kdm/icon_lower_f1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hoth/icon_lower_male
        rgbGen identity
    }
    {
        map models/players/jedi_hoth/icon_lower_male
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}


//Twi'lek Female Icons
//This is the type of shader for icons that don't have color tinted areas.
//These are normally stored in jpeg format to save space.
models/players/jedi_tf/icon_torso_g1
{
	nopicmip
	nomipmaps
	q3map_nolightmap
    {
        map models/players/jedi_tf/icon_torso_g1
    }
}

models/players/jedi_tf/icon_lower_e1
{
	nopicmip
	nomipmaps
	q3map_nolightmap
    {
        map models/players/jedi_tf/icon_lower_e1
    }
}


//Rodian Male Icons
models/players/jedi_rm/icon_torso_h1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_rm/icon_torso_h1
        rgbGen identity
    }
    {
        map models/players/jedi_rm/icon_torso_h1
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}

models/players/jedi_rm/icon_lower_f1
{
	nopicmip
	nomipmaps
    {
        map models/players/jedi_hoth/icon_lower_male
        rgbGen identity
    }
    {
        map models/players/jedi_hoth/icon_lower_male
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}



//Player Model Texture Shaders

//Male Player Textures
models/players/jedi_hoth/male_armspc
{
    {
        map models/players/jedi_hoth/male_armspc
        blendFunc GL_ONE GL_ZERO
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hoth/male_armspc
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hoth/male_torsopc
{
    {
        map models/players/jedi_hoth/male_torsopc
        blendFunc GL_ONE GL_ZERO
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hoth/male_torsopc
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hoth/male_legspc
{
    {
        map models/players/jedi_hoth/male_legspc
        blendFunc GL_ONE GL_ZERO
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hoth/male_legspc
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}


//Oooohh!  Females!
models/players/jedi_hoth/female_armspc
{
    {
        map models/players/jedi_hoth/female_armspc
        blendFunc GL_ONE GL_ZERO
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hoth/female_armspc
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hoth/female_torsopc
{
    {
        map models/players/jedi_hoth/female_torsopc
        blendFunc GL_ONE GL_ZERO
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hoth/female_torsopc
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}

models/players/jedi_hoth/female_legspc
{
    {
        map models/players/jedi_hoth/female_legspc
        blendFunc GL_ONE GL_ZERO
        rgbGen lightingDiffuseEntity
    }
    {
        map models/players/jedi_hoth/female_legspc
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen lightingDiffuse
    }
}
