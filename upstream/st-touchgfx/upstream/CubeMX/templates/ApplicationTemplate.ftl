[#ftl]
[#list SWIPdatas as data]
[#if data.parameters??]

[#assign PixelFormat = data.parameters.tgfx_pixel_format.split(":")?first]
[#assign BitsPerPixel = data.parameters.tgfx_pixel_format.split(":")?last]

[#--  Project Name  --]
[#list configs as config]
[#assign ProjectName = config.projectName]
[#assign Family = config.family]
[#assign InternalRam = config.mcuParams.Ram]
[#if config.mcuParams.Flash??]
[#assign InternalFlash = config.mcuParams.Flash]
[/#if]
[/#list]

[#--  Default LCD class --]
[#assign LCD = "LCD${BitsPerPixel}bpp"]

[#--  Overwrite if LCD8 type --]
[#if PixelFormat.contains("2222")]
[#assign LCD = "LCD8bpp_${PixelFormat}"]
[/#if]

[#--  Append if Data Reader is enabled --]
[#if data.parameters.tgfx_datareader?? && data.parameters.tgfx_datareader == "Enabled"]
[#assign LCD = "${LCD}SerialFlash"]
[/#if]

[#--  Determine which width and height to use --]
[#assign Width = 0]
[#assign Height = 0]
[#if data.parameters.tgfx_display_interface == "disp_ltdc"]
[#assign Width = data.parameters.tgfx_ltdc_width]
[#assign Height = data.parameters.tgfx_ltdc_height]
[#else]
[#assign Width = data.parameters.tgfx_custom_width]
[#assign Height = data.parameters.tgfx_custom_height]
[/#if]
{
  "Application": {
    "Name": "${ProjectName}",
    "TouchGfxPath": "../Middlewares/ST/touchgfx",
    "AvailableColorDepths": [ ${BitsPerPixel} ],
    "AvailableLCDs":
    {
      "${BitsPerPixel}": "${LCD}"
    },
    "AvailableResolutions" : [
      {
        "Width": ${Width},
        "Height": ${Height}
      }
    ],
    "PostGenerateTargetCommand" : "touchgfx update_project --project-file=../${ProjectName}.ioc[#if data.parameters.tgfx_mcu_family??] --platform=${data.parameters.tgfx_mcu_family}[/#if]",
    "Family" : "${Family}"
  },
  "Version": "${data.version}"
}
[/#if]
[/#list]