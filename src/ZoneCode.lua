ZoneCode = {}

ZoneCode.Assets = {
    T6 = {
        "PhysPreset",
        "PhysConstraints",
        "DestructibleDef",
        "XAnimParts",
        "XModel",
        "Material",
        "MaterialTechniqueSet",
        "GfxImage",
        "SndBank",
        "SndPatch",
        "clipMap_t",
        "ComWorld",
        "GameWorldSp",
        "GameWorldMp",
        "MapEnts",
        "GfxWorld",
        "GfxLightDef",
        "Font_s",
        "FontIcon",
        "MenuList",
        "menuDef_t",
        "LocalizeEntry",
        "WeaponVariantDef",
        "WeaponAttachment",
        "WeaponAttachmentUnique",
        "WeaponCamo",
        "SndDriverGlobals",
        "FxEffectDef",
        "FxImpactTable",
        "RawFile",
        "StringTable",
        "LeaderboardDef",
        "XGlobals",
        "ddlRoot_t",
        "Glasses",
        "EmblemSet",
        "ScriptParseTree",
        "KeyValuePairs",
        "VehicleDef",
        "MemoryBlock",
        "AddonMapEnts",
        "TracerDef",
        "SkinnedVertsDef",
        "Qdb",
        "Slug",
        "FootstepTableDef",
        "FootstepFXTableDef",
        "ZBarrierDef"
    }
}

function ZoneCode:outputForAssets(assetList)
    buildoutputs {
        "%{wks.location}/src/ZoneCode/Game/%{file.basename}/AssetLoader%{file.basename}.h",
        "%{wks.location}/src/ZoneCode/Game/%{file.basename}/AssetMarker%{file.basename}.h",
        "%{wks.location}/src/ZoneCode/Game/%{file.basename}/AssetWriter%{file.basename}.h",
    }

    for i = 1, #assetList do
        local assetNameLower = string.lower(assetList[i])
        buildoutputs {
            "%{wks.location}/src/ZoneCode/Game/%{file.basename}/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_%{string.lower(file.basename)}_load_db.cpp",
            "%{wks.location}/src/ZoneCode/Game/%{file.basename}/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_%{string.lower(file.basename)}_load_db.h",
            "%{wks.location}/src/ZoneCode/Game/%{file.basename}/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_%{string.lower(file.basename)}_mark_db.cpp",
            "%{wks.location}/src/ZoneCode/Game/%{file.basename}/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_%{string.lower(file.basename)}_mark_db.h",
            "%{wks.location}/src/ZoneCode/Game/%{file.basename}/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_%{string.lower(file.basename)}_write_db.cpp",
            "%{wks.location}/src/ZoneCode/Game/%{file.basename}/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_%{string.lower(file.basename)}_write_db.h",
            "%{wks.location}/src/ZoneCode/Game/%{file.basename}/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_%{string.lower(file.basename)}_struct_test.cpp",
        }
    end
end

function ZoneCode:allTestFiles()
    result = {}

    for game, assets in pairs(self.Assets) do
        local gameLower = string.lower(game)

        for i, assetName in ipairs(assets) do
            local assetNameLower = string.lower(assetName)
            table.insert(result, "%{wks.location}/src/ZoneCode/Game/" .. game .. "/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_" .. gameLower .. "_struct_test.cpp")
        end
    end
    
    return result
end

function ZoneCode:allMarkFiles()
    result = {}

    for game, assets in pairs(self.Assets) do
        local gameLower = string.lower(game)

        -- PerAsset
        for i, assetName in ipairs(assets) do
            local assetNameLower = string.lower(assetName)
            table.insert(result, "%{wks.location}/src/ZoneCode/Game/" .. game .. "/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_" .. gameLower .. "_mark_db.cpp")
            table.insert(result, "%{wks.location}/src/ZoneCode/Game/" .. game .. "/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_" .. gameLower .. "_mark_db.h")
        end
        
        -- PerTemplate
        table.insert(result, "%{wks.location}/src/ZoneCode/Game/" .. game .. "/AssetMarker" .. game .. ".h")
    end
    
    return result
end

function ZoneCode:allLoadFiles()
    result = {}

    for game, assets in pairs(self.Assets) do
        local gameLower = string.lower(game)

        -- PerAsset
        for i, assetName in ipairs(assets) do
            local assetNameLower = string.lower(assetName)
            table.insert(result, "%{wks.location}/src/ZoneCode/Game/" .. game .. "/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_" .. gameLower .. "_load_db.cpp")
            table.insert(result, "%{wks.location}/src/ZoneCode/Game/" .. game .. "/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_" .. gameLower .. "_load_db.h")
        end
        
        -- PerTemplate
        table.insert(result, "%{wks.location}/src/ZoneCode/Game/" .. game .. "/AssetLoader" .. game .. ".h")
    end
    
    return result
end

function ZoneCode:allWriteFiles()
    result = {}

    for game, assets in pairs(self.Assets) do
        local gameLower = string.lower(game)

        -- PerAsset
        for i, assetName in ipairs(assets) do
            local assetNameLower = string.lower(assetName)
            table.insert(result, "%{wks.location}/src/ZoneCode/Game/" .. game .. "/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_" .. gameLower .. "_write_db.cpp")
            table.insert(result, "%{wks.location}/src/ZoneCode/Game/" .. game .. "/XAssets/" .. assetNameLower .. "/" .. assetNameLower .. "_" .. gameLower .. "_write_db.h")
        end
        
        -- PerTemplate
        table.insert(result, "%{wks.location}/src/ZoneCode/Game/" .. game .. "/AssetWriter" .. game .. ".h")
    end
    
    return result
end

function ZoneCode:include(includes)
	if includes:handle(self:name()) then
		Common:include(includes)
        includedirs {
            path.join(ProjectFolder(), "ZoneCode"),
            "%{wks.location}/src/ZoneCode"
        }
    end
end

function ZoneCode:link(links)
    
end

function ZoneCode:use()
	dependson(self:name())
end

function ZoneCode:name()
    return "ZoneCode"
end

function ZoneCode:project()
	local folder = ProjectFolder()
	local includes = Includes:create()

	project(self:name())
        targetdir(TargetDirectoryLib)
		location "%{wks.location}/src/%{prj.name}"
		kind "Utility"
		
		files {
			path.join(folder, "ZoneCode/**.gen"),
			path.join(folder, "ZoneCode/**.h"),
			path.join(folder, "ZoneCode/**.txt")
        }
		
        vpaths {
			["*"] = {
				path.join(folder, "ZoneCode")
			}
		}

        self:include(includes)
        ZoneCodeGenerator:use()

        filter "files:**.gen"
            buildmessage "Generating ZoneCode for game %{file.basename}"
            buildcommands {
                '"' .. TargetDirectoryBuildTools .. '/' .. ExecutableByOs('ZoneCodeGenerator') .. '"' 
                    .. ' --no-color'
                    .. ' -h "' .. path.join(path.getabsolute(ProjectFolder()), 'ZoneCode/Game/%{file.basename}/%{file.basename}_ZoneCode.h') .. '"'
                    .. ' -c "' .. path.join(path.getabsolute(ProjectFolder()), 'ZoneCode/Game/%{file.basename}/%{file.basename}_Commands.txt') .. '"'
                    .. ' -o "%{wks.location}/src/ZoneCode/Game/%{file.basename}"'
                    .. ' -g ZoneLoad'
                    .. ' -g ZoneMark'
                    .. ' -g ZoneWrite'
                    .. ' -g AssetStructTests'
            }
            buildinputs {
                path.join(ProjectFolder(), "ZoneCode/Game/%{file.basename}/%{file.basename}_ZoneCode.h"),
                path.join(ProjectFolder(), "ZoneCode/Game/%{file.basename}/%{file.basename}_Commands.txt"),
                path.join(ProjectFolder(), "Common/Game/%{file.basename}/%{file.basename}_Assets.h"),
                TargetDirectoryBuildTools .. "/" .. ExecutableByOs('ZoneCodeGenerator')
            }
        filter {}
        
        filter "files:**/T6.gen"
            self:outputForAssets(self.Assets.T6)
        filter {}
end
