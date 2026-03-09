# Supported Asset Types

All asset types are supported to be loaded from other fastfiles in memory.
The following section specify which assets are supported to be dumped to disk (using `Unlinker`) and loaded from disk (using `Linker`):

- ❌ = Currently unsupported
- ✅ = Supported
- ⁉️ = Partial support

| Asset Type             | Dumping Support | Loading Support | Notes                                                                                       |
|------------------------|-----------------|-----------------|---------------------------------------------------------------------------------------------|
| PhysPreset             | ✅              | ✅              |                                                                                             |
| PhysConstraints        | ✅              | ✅              |                                                                                             |
| DestructibleDef        | ✅              | ❌              |                                                                                             |
| XAnimParts             | ⁉️              | ❌              |                                                                                             |
| XModel                 | ✅              | ✅              | Model data can be exported to `XMODEL_EXPORT/XMODEL_BIN`, `OBJ`, `GLB/GLTF`.               |
| Material               | ✅              | ✅              | Dumping/Loading currently works for compiled materials. No material pipeline yet.          |
| MaterialTechniqueSet   | ✅              | ✅              | Only dumps compiled shaders.                                                                |
| GfxImage               | ✅              | ✅              | Some special image encodings are not yet supported.                                         |
| SndBank                | ✅              | ✅              | The current implementation is subject to change.                                            |
| SndPatch               | ❌              | ❌              |                                                                                             |
| clipMap_t              | ⁉️              | ❌              | Only dumps dynamic models.                                                                  |
| ComWorld               | ✅              | ❌              |                                                                                             |
| GameWorldSp            | ✅              | ❌              |                                                                                             |
| GameWorldMp            | ✅              | ❌              |                                                                                             |
| MapEnts                | ✅              | ✅              | Currently doesn't dump triggers.                                                            |
| GfxWorld               | ⁉️              | ❌              | Only dumps reflection probes and static models.                                             |
| GfxLightDef            | ✅              | ✅              | Loader currently only supports `.json`.                                                     |
| Font_s                 | ✅              | ❌              |                                                                                             |
| FontIcon               | ✅              | ✅              |                                                                                             |
| MenuList               | ✅              | ✅              |                                                                                             |
| menuDef_t              | ⁉️              | ❌              | Dumper is a work in progress.                                                               |
| LocalizeEntry          | ✅              | ✅              |                                                                                             |
| WeaponVariantDef       | ✅              | ✅              |                                                                                             |
| WeaponAttachment       | ✅              | ✅              |                                                                                             |
| WeaponAttachmentUnique | ✅              | ✅              |                                                                                             |
| WeaponCamo             | ✅              | ✅              |                                                                                             |
| SndDriverGlobals       | ✅              | ❌              |                                                                                             |
| FxEffectDef            | ❌              | ❌              |                                                                                             |
| FxImpactTable          | ❌              | ❌              |                                                                                             |
| RawFile                | ✅              | ✅              |                                                                                             |
| StringTable            | ✅              | ✅              |                                                                                             |
| LeaderboardDef         | ✅              | ✅              |                                                                                             |
| XGlobals               | ❌              | ❌              |                                                                                             |
| ddlRoot_t              | ❌              | ❌              |                                                                                             |
| Glasses                | ❌              | ❌              |                                                                                             |
| EmblemSet              | ❌              | ❌              |                                                                                             |
| ScriptParseTree        | ⁉️              | ⁉️              | Binary only. Editing possible with GSC-Tool. Plutonium supports loading source files.      |
| KeyValuePairs          | ✅              | ✅              | Dumped/loaded as part of the `.zone` file.                                                  |
| VehicleDef             | ✅              | ✅              |                                                                                             |
| MemoryBlock            | ❌              | ❌              |                                                                                             |
| AddonMapEnts           | ✅              | ✅              |                                                                                             |
| TracerDef              | ✅              | ✅              |                                                                                             |
| SkinnedVertsDef        | ✅              | ❌              |                                                                                             |
| Qdb                    | ⁉️              | ⁉️              | Dumped/loaded as rawfiles. Usage currently unknown.                                         |
| Slug                   | ⁉️              | ⁉️              | Dumped/loaded as rawfiles. Usage currently unknown.                                         |
| FootstepTableDef       | ❌              | ❌              |                                                                                             |
| FootstepFXTableDef     | ❌              | ❌              |                                                                                             |
| ZBarrierDef            | ✅              | ✅              |                                                                                             |
