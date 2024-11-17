/* Copyright (C) 2021 Mr Goldberg
   This file is part of the Goldberg Emulator

   The Goldberg Emulator is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   The Goldberg Emulator is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the Goldberg Emulator; if not, see
   <http://www.gnu.org/licenses/>.  */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <ShlObj.h>
#include <io.h>
#include <fcntl.h>
#include <queue>
#include <list>
#include <filesystem>
#include <string>
#include <objbase.h>
#include "upc.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
std::string get_full_lib_path()
{
    std::string program_path;
#if defined(_WIN32)
    char   DllPath[MAX_PATH] = {0};
    GetModuleFileName((HINSTANCE)&__ImageBase, DllPath, _countof(DllPath));
    program_path = DllPath;
#else
    program_path = get_lib_path();
#endif
    return program_path.substr(0, program_path.rfind("\\")).append("\\");
}

#ifdef EMU_RELEASE_BUILD
#define PRINT_DEBUG(a, ...);
#else
#define PRINT_DEBUG(a, ...) do {FILE *t = fopen("uplay_LOG.txt", "a"); fprintf(t, "%u " a, GetCurrentThreadId(), __VA_ARGS__); fclose(t);} while (0)
#endif

#define EXPORT_FUNC extern "C" _declspec(dllexport)
struct cb_data {
    cb_data(void *fn, void *context_data, unsigned long arg) {
        this->cb = (void (*)(unsigned long, void *))fn;
        this->context_data = context_data;
        this->arg1 = arg;
    }

    void (*cb)(unsigned long, void *);
    unsigned long arg1;
    void *context_data;
};

#define EXTRA_SAVE_PADDING (32 + 8)
struct emu_config {
    std::string name;
    std::string email;
    std::string uplay_id;
    std::string language;
    std::string ticket;
    std::string game_appid;
    std::filesystem::path save_directory;
    uint32_t appid;
    std::list<uint32_t> dlcs;
    std::list<uint32_t> items;
};

struct context_data {
    bool initialized;
    std::queue<cb_data> cb;
    emu_config config;
};
static emu_config emulator_config;


EXPORT_FUNC const char * UPC_ErrorToString(int param_1)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    switch(param_1) {
    case -0xe:
        return "Unavailable";
    case -0xd:
        return "Failed precondition";
    default:
        return "Unknown error";
    case -0xb:
        return "Operation aborted";
    case -10:
        return "Internal error";
    case -9:
        return "Unauthorized action";
    case -7:
        return "End of file";
    case -6:
        return "Not found";
    case -5:
        return "Memory error";
    case -4:
        return "Communication error";
    case -3:
        return "Uninitialized subsystem";
    case -2:
        return "Invalid arguments";
    case -1:
        return "Declined";
    }
}


EXPORT_FUNC int UPC_AchievementImageFree(void *context, void *inImageRGBA)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_AchievementImageGet(void *context, unsigned inId, void *outImageRGBA, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_AchievementListFree(void *context, void *inAchievementList)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_AchievementListGet(void *context, void *inOptUserIdUtf8, unsigned inFilter, void *outAchievementList, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    data->cb.push(cb_data(inCallback, inCallbackData, -4)); //No achivements when offline
    return 0x2000;
}

EXPORT_FUNC int UPC_AchievementUnlock(void *context, unsigned inId, void *inOptCallback, void *inOptCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_ApplicationIdGet(void *context, const char **outAppId)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    *outAppId = data->config.game_appid.c_str();
    return 0;
}

EXPORT_FUNC int UPC_AvatarFree(void *context, void *inImageRGBA)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_AvatarGet(void *context, const char *inOptUserIdUtf8, unsigned inSize, void *outImageRGBA, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s %s %d\n", __FUNCTION__, inOptUserIdUtf8, inSize);

    unsigned properSize = 4000; //64x64
    if (inSize == 1)
        properSize = 10000; // 128x128
    if (inSize == 2)
        properSize = 40000; // 256x256
    // read from file and drop into out

    return 0;
}

EXPORT_FUNC int UPC_BlacklistAdd(void *context, void *inUserIdUtf8, void *inOptCallback, void *inOptCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_BlacklistHas(void *context, void *inUserIdUtf8)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_BlacklistHas_Extended(void *context, void *inUserIdUtf8, void *outIsBlacklisted)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC void UPC_Cancel(void *context, int inHandle)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);

}


EXPORT_FUNC context_data *UPC_ContextCreate(unsigned inVersion, void *inOptSetting)
{
    PRINT_DEBUG("%s %u %p\n", __FUNCTION__, inVersion, inOptSetting);
    context_data *data = new context_data();
    data->config = emulator_config;
    return data;
}

EXPORT_FUNC int UPC_ContextFree(context_data *context)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    delete context;
    return 0;
}

EXPORT_FUNC int UPC_CPUScoreGet(void *context, unsigned *outScore)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    if (outScore) *outScore = 0x1000;
    return 0;
}

EXPORT_FUNC const char *UPC_EmailGet(void *context)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    return data->config.email.c_str();
}

EXPORT_FUNC int UPC_EmailGet_Extended(void *context, const char **outEmail)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    *outEmail = data->config.email.c_str();
    return 0;
}

EXPORT_FUNC int UPC_EventNextPeek(void *context, void *outEvent)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_EventNextPoll(void *context, void *outEvent)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return -6;
}

EXPORT_FUNC int UPC_EventRegisterHandler(void *context, unsigned inType, void *inHandler, void *inOptData)
{
    PRINT_DEBUG("%s %u %p %p\n", __FUNCTION__, inType, inHandler, inOptData); //inHandler is function
    return 0;
}

EXPORT_FUNC int UPC_FriendAdd(void *context, void *inSearchStringUtf8, void *inOptCallback, void *inOptCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_FriendCheck(void *context, char *inUserIdUtf8)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_FriendCheck_Extended(void *context, char *inUserIdUtf8, void *outIsFriend)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_FriendListFree(void *context, void *inFriendList)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_FriendListGet(void *context, unsigned inOptOnlineStatusFilter, void *outFriendList, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return -0xD;
    //00000000FFFFFFF3 when offline
    return 0;
}

EXPORT_FUNC int UPC_FriendRemove(void *context, void *inUserIdUtf8, void *inOptCallback, void *inOptCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_GPUScoreGet(void *context, unsigned *outScore, float *outConfidenceLevel)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    if (outScore) *outScore = 0x1000;
    if (outConfidenceLevel) *outConfidenceLevel = 1.0;
    return 0;
}

EXPORT_FUNC const char *UPC_IdGet(void *context)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    return data->config.uplay_id.c_str();
}

EXPORT_FUNC int UPC_IdGet_Extended(void *context, const char **outId)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    *outId = data->config.uplay_id.c_str();
    return 0;
}

static std::string get_profile_string(LPCSTR name, LPCSTR key, LPCSTR def, LPCSTR filename)
{
    char temp[1024];
    int result = GetPrivateProfileStringA(name, key, def, temp, sizeof(temp), filename);
    return std::string(temp, result);
}
static std::string GUIDToString(GUID *guid) {
    char guid_string[37]; // 32 hex chars + 4 hyphens + null terminator
    snprintf(
          guid_string, sizeof(guid_string),
          "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
          guid->Data1, guid->Data2, guid->Data3,
          guid->Data4[0], guid->Data4[1], guid->Data4[2],
          guid->Data4[3], guid->Data4[4], guid->Data4[5],
          guid->Data4[6], guid->Data4[7]);
    return guid_string;
}

EXPORT_FUNC int UPC_Init(unsigned inVersion, int appid)
{
    PRINT_DEBUG("%s %u %i\n", __FUNCTION__, inVersion, appid);
    std::string lib_path = get_full_lib_path();
    std::string ini_path = lib_path + "\\uplay_r2.ini";
    int save_type = GetPrivateProfileIntA("Settings", "SaveType", 0, ini_path.c_str());

    if (save_type == 1) {
        emulator_config.save_directory = lib_path;
        emulator_config.save_directory /= get_profile_string("Settings", "SavePath", "SAVE_GAMES", ini_path.c_str());
    } else if (save_type == 2) {
        emulator_config.save_directory = get_profile_string("Settings", "SavePath", "SAVE_GAMES", ini_path.c_str());
    } else {
        std::string user_appdata_path = "SAVE";
        CHAR szPath[MAX_PATH] = {};

        HRESULT hr = SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, szPath);

        if (SUCCEEDED(hr)) {
            user_appdata_path = szPath;
        }

        emulator_config.save_directory = user_appdata_path;
        emulator_config.save_directory /= "Goldberg UplayEmu Saves";
    }

    emulator_config.save_directory /= std::to_string(appid);
    std::filesystem::create_directories(emulator_config.save_directory);

    emulator_config.email = get_profile_string("Settings", "Email", "gold@berg", ini_path.c_str());
    emulator_config.language = get_profile_string("Settings", "Language", "en-US", ini_path.c_str());
    emulator_config.name = get_profile_string("Settings", "Username", "Mr_Goldberg", ini_path.c_str());
    emulator_config.uplay_id = get_profile_string("Settings", "UserId", "80f33a39-e682-4d1f-b693-39267e890df2", ini_path.c_str());
    emulator_config.game_appid = get_profile_string("Settings", "GameId", "66333333-e688-4d1f-b693-39267e890df2", ini_path.c_str());
    emulator_config.ticket = "";
    emulator_config.appid = appid;

    std::string generateNewId = get_profile_string("Settings", "GenerateNewId", "false", ini_path.c_str());

    if (generateNewId == "true")
    {
        GUID gidReference;
        HRESULT hCreateGuid = CoCreateGuid( &gidReference );
        emulator_config.uplay_id = GUIDToString(&gidReference);
        WritePrivateProfileStringA("Settings", "GenerateNewId", "false", ini_path.c_str());
        WritePrivateProfileStringA("Settings", "UserId", emulator_config.uplay_id.c_str(), ini_path.c_str());
    }


    {
            char depots_raw[32767] = {};
            GetPrivateProfileSectionA("DLC", depots_raw, sizeof(depots_raw), ini_path.c_str());
            char *dlc_loop = depots_raw;
            while (*dlc_loop) {
                std::string line = std::string(dlc_loop);
                try {
                    uint32_t dlc_id = stoul(line);
                    emulator_config.dlcs.push_back(dlc_id);
                } catch (...) {}
                dlc_loop += line.size() + 1;
            }
    }

    {
            char depots_raw[32767] = {};
            GetPrivateProfileSectionA("Items", depots_raw, sizeof(depots_raw), ini_path.c_str());
            char *dlc_loop = depots_raw;
            while (*dlc_loop) {
                std::string line = std::string(dlc_loop);
                try {
                    uint32_t dlc_id = stoul(line);
                    emulator_config.items.push_back(dlc_id);
                } catch (...) {}
                dlc_loop += line.size() + 1;
            }
    }

    PRINT_DEBUG("%s %s %s %s %s\n", emulator_config.save_directory.string().c_str(), emulator_config.email.c_str(), emulator_config.language.c_str(), emulator_config.name.c_str(), emulator_config.uplay_id.c_str());
    return 0;
}


EXPORT_FUNC int UPC_InstallChunkListFree(void *context, UPC_InstallChunkList *aChunkList)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    if (!context || !aChunkList) return -0xd;
    if (aChunkList->list) delete[] aChunkList->list;
    delete aChunkList;
    return 0;
}

EXPORT_FUNC int UPC_InstallChunkListGet(void *context, UPC_InstallChunkList **outChunkList)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    if (!context || !outChunkList) return -0xd;
    UPC_InstallChunkList *chunks = new UPC_InstallChunkList();
    chunks->count = 0;
    chunks->list = NULL;
    *outChunkList = chunks;
    return 0;
}

EXPORT_FUNC int UPC_InstallChunksOrderUpdate(void *context, void *inChunkIds, unsigned inChunkCount)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_InstallChunksOrderUpdate_Extended(void *context, void *inChunkIds, unsigned inChunkCount, void *unknown1, void *unknown2)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_InstallChunksPresenceCheck(void *context, void *inChunkIds, unsigned inChunkCount)
{
    PRINT_DEBUG("%s %p %u\n", __FUNCTION__, inChunkIds, inChunkCount);
    return 0;
}

EXPORT_FUNC const char *UPC_InstallLanguageGet(void *context)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    return data->config.language.c_str();
}

EXPORT_FUNC int UPC_InstallLanguageGet_Extended(void *context, const char **outInstallLanguage)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    *outInstallLanguage = data->config.language.c_str();
    return 0;
}

EXPORT_FUNC int UPC_IsCrossBootAllowed(void *context, unsigned inProductId, bool *outIsCrossBootAllowed, void * a4, void * a5)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    *outIsCrossBootAllowed = false;
    return 0;
}

//EXPORT_FUNC int UPC_LaunchApp(void *context, unsigned inProductId, void *??? always zero)
EXPORT_FUNC int UPC_LaunchApp(void *context, unsigned inProductId, void * MustBeZero)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 1;
}

EXPORT_FUNC int UPC_MultiplayerInvite(void *context, void *inUserIdUtf8, void *inOptCallback, void *inOptCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_MultiplayerInviteAnswer(void *context, void *inSenderIdUtf8, unsigned inIsAccepted, void *inOptCallback, void *inOptCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_MultiplayerSessionClear(void *context)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_MultiplayerSessionClear_Extended(void *context, void *unk_0, void *unk_1)
{
    PRINT_DEBUG("%s %p %p\n", __FUNCTION__, unk_0, unk_1);
    return 0;
}

EXPORT_FUNC int UPC_MultiplayerSessionFree(void *context, void *inMultiplayerSession)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_MultiplayerSessionGet(void *context, void *outMultiplayerSession)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_MultiplayerSessionSet(void *context, void *inMultiplayerSession)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_MultiplayerSessionSet_Extended(void *context, void *inMultiplayerSession, void *unk_0, void *unk_1)
{
    PRINT_DEBUG("%s %p %p\n", __FUNCTION__, unk_0, unk_1);
    return 0;
}

EXPORT_FUNC const char *UPC_NameGet(void *context)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    return data->config.name.c_str();
}

EXPORT_FUNC int UPC_NameGet_Extended(void *context, const char **outName)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    *outName = data->config.name.c_str();
    return 0;
}

EXPORT_FUNC int UPC_OverlayFriendInvitationShow(void *context, void *inOptIdListUtf8, unsigned inOptIdListLength)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_OverlayFriendInvitationShow_Extended(void *context,  void *inOptIdListUtf8, unsigned inOptIdListLength, void *unk_0, void *unk_1)
{
    PRINT_DEBUG("%s %p %p\n", __FUNCTION__, unk_0, unk_1);
    return 0;
}
EXPORT_FUNC int UPC_OverlayFriendSelectionFree(void *context, void *inSelectedFriends)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_OverlayFriendSelectionShow(void *context, void *inIdListUtf8, unsigned inIdListLength, void *outSelectedFriends, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_OverlayMicroAppShow(void *context, void *inAppName, void *inOptMicroAppParamList, void * inCallback, void *inOptCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_OverlayNotificationShow(void *context, unsigned inId)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_OverlayNotificationShow_Extended(void *context, unsigned inId, void *unk_0, void *unk_1)
{
    PRINT_DEBUG("%s %p %p\n", __FUNCTION__, unk_0, unk_1);
    return 0;
}

EXPORT_FUNC int UPC_OverlayShow(void *context, unsigned inSection, void *inOptCallback, void *inOptCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}


EXPORT_FUNC int UPC_ProductConsume(void *context, unsigned inProductId, unsigned inQuantity, void *inTransactionIdUtf8, void *inSignatureUtf8, void *outResponseSignatureUtf8, void *inCallback, void *inOptCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_ProductConsumeSignatureFree(void *context, void *inResponseSignature)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}


EXPORT_FUNC int UPC_ProductListFree(void *context, UPC_ProductList *inProductList)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    if (inProductList) {
        for (unsigned i = 0; i < inProductList->count; ++i) {
            delete inProductList->list[i];
        }

        delete[] inProductList->list;
    }

    delete inProductList;
    return 0;
}

EXPORT_FUNC int UPC_ProductListGet(void *context, char *inOptUserIdUtf8, unsigned inFilter, UPC_ProductList **outProductList, void *inCallback, void *inCallbackData) //CB: 1 argument, 0 val
{
    PRINT_DEBUG("%s %p %s %u %p %p %p\n", __FUNCTION__, context, inOptUserIdUtf8, inFilter, outProductList, inCallback, inCallbackData);
    context_data *data = (context_data *)context;
    data->cb.push(cb_data(inCallback, inCallbackData, 0));
    std::list<UPC_Product> products;
    products.push_back(UPC_Product(data->config.appid, 1));
    for (auto & dlc : data->config.dlcs) {
        products.push_back(UPC_Product(dlc, 2));
    }

    for (auto & item : data->config.items) {
        products.push_back(UPC_Product(item, 4));
    }
    // for (unsigned i = 0; i < 200; ++i) {
    //     products.push_back(product(data->config.appid + i, 2));
    // }
    //products.push_back(product(0x1572, 2)); //fenyx dlc 1

    unsigned index = 0;
    UPC_ProductList *pr_list = new UPC_ProductList();
    pr_list->list = new UPC_Product*[products.size()];
    for (auto & p: products) {
        pr_list->list[index] = new UPC_Product(p);
        ++index;
    }

    pr_list->count = index;
    *outProductList = pr_list;
    return 0x10000;
}

EXPORT_FUNC int UPC_RichPresenceSet(void *context, unsigned inId, void *inOptTokenList)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_RichPresenceSet_Extended(void *context, unsigned inId, void *inOptTokenList, void *unk_0, void *unk_1)
{
    PRINT_DEBUG("%s %p %p\n", __FUNCTION__, unk_0, unk_1);
    return 0;
}

EXPORT_FUNC int UPC_ShowBrowserUrl(void *context, void *inBrowserUrlUtf8)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StorageFileClose(void *context, unsigned inHandle)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return _close(inHandle);
    //return 0;
}

EXPORT_FUNC int UPC_StorageFileDelete(void *context, char *inFileNameUtf8)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    if (std::filesystem::remove(data->config.save_directory / inFileNameUtf8))
        return 0;
    else
        return -1;
}


EXPORT_FUNC int UPC_StorageFileListFree(void *context, UPC_StorageFileList *inStorageFileList)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    if (!inStorageFileList) return -0xd;
    if (inStorageFileList->count) {
        for (int i = 0; i < inStorageFileList->count; ++i) {
            delete[] inStorageFileList->list[i]->fileNameUtf8;
            delete[] inStorageFileList->list[i]->legacyNameUtf8;
            delete inStorageFileList->list[i];
        }
    }

    delete[] inStorageFileList->list;
    delete inStorageFileList;
    return 0;
}

EXPORT_FUNC int UPC_StorageFileListGet(void *context, UPC_StorageFileList **outStorageFileList)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    if (!context || !outStorageFileList) return -0xd;
    context_data *data = (context_data *)context;

    std::list<std::filesystem::directory_entry> list;
    for (const auto & file : std::filesystem::directory_iterator(data->config.save_directory)) {
        if (file.is_regular_file()) {
            list.push_back(file);
        }
    }

    UPC_StorageFileList *fl = new UPC_StorageFileList();
    fl->count = list.size();
    fl->list = new UPC_StorageFile*[fl->count];
    unsigned index = 0;
    for (auto & file : list) {
        unsigned long file_size = std::filesystem::file_size(file);
        std::string file_name = file.path().filename().string();
#ifdef NO_PADDING
        if (file_size < EXTRA_SAVE_PADDING) {
            --fl->count;
            continue;
        }
#endif
        UPC_StorageFile *f_data = new UPC_StorageFile();
        f_data->fileNameUtf8 = new char[file_name.size() + 1];
        file_name.copy(f_data->fileNameUtf8, file_name.size());
        f_data->fileNameUtf8[file_name.size()] = 0;
        file_name.copy(f_data->legacyNameUtf8, file_name.size());
        f_data->legacyNameUtf8[file_name.size()] = 0;
#ifdef NO_PADDING
        f_data->size = file_size;
#else
        f_data->size = file_size - EXTRA_SAVE_PADDING;
#endif
        f_data->lastModifiedMs = std::chrono::duration_cast<std::chrono::milliseconds>(file.last_write_time().time_since_epoch()).count();
        fl->list[index] = f_data;
        ++index;
    }

    *outStorageFileList = fl;
    return 0;
}

EXPORT_FUNC int UPC_StorageFileOpen(void *context, char *inFileNameUtf8, unsigned inFlags, int *outHandle)
{
    PRINT_DEBUG("%s %s %u\n", __FUNCTION__, inFileNameUtf8, inFlags);
    UPC_FileOpenMode flags = (UPC_FileOpenMode)inFlags;
    int oflag = _O_BINARY | _O_CREAT | _O_RDWR;
    if (flags == UPC_FileOpenMode_Read) oflag |= _O_TRUNC;
    context_data *data = (context_data *)context;
    int file_handle = _open((data->config.save_directory / inFileNameUtf8).string().c_str(), oflag, _S_IREAD | _S_IWRITE);
    *outHandle = file_handle;
    return 0;
}

EXPORT_FUNC int UPC_StorageFileRead(void *context, int inHandle, int inBytesToRead, unsigned inBytesReadOffset, void *outData, int *outBytesRead, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s %i %i %u\n", __FUNCTION__, inHandle, inBytesToRead, inBytesReadOffset);
    context_data *data = (context_data *)context;
#ifdef NO_PADDING
    _lseek(inHandle, inBytesReadOffset, SEEK_SET);
#else
    _lseek(inHandle, inBytesReadOffset + EXTRA_SAVE_PADDING, SEEK_SET);
#endif
    int ret = _read(inHandle, outData, inBytesToRead);
    int out = 0;
    if (ret >= 0) {
        *outBytesRead = ret;
    } else {
        out = -1; //not sure if right error
    }
    data->cb.push(cb_data(inCallback, inCallbackData, out));
    return 0x10000;
}

EXPORT_FUNC int UPC_StorageFileWrite(void *context, int inHandle, void *inData, int inSize, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s %i %i\n", __FUNCTION__, inHandle, inSize);
    context_data *data = (context_data *)context;
#ifndef NO_PADDING
    char padding_data[EXTRA_SAVE_PADDING] = {};
    _write(inHandle, padding_data, sizeof(padding_data));
#endif
    int ret = _write(inHandle, inData, inSize);
    int out = 0;
    if (ret < 0) {
        out = -1; //Not sure if right error
    }
    data->cb.push(cb_data(inCallback, inCallbackData, out));
    return 0x10000;
}

EXPORT_FUNC int UPC_StoreCheckout(void *context, unsigned inId)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC bool UPC_StoreIsEnabled(void *context)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return true;
}

EXPORT_FUNC int UPC_StoreIsEnabled_Extended(void *context, int *outEnabled)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    *outEnabled = true;
    return 0;
}

EXPORT_FUNC int UPC_StoreLanguageSet(void *context, char *inLanguageCountryCode)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StorePartnerGet(void *context)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StorePartnerGet_Extended(void *context, void *outStorePartner)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StoreProductDetailsShow(void *context, unsigned inId)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StoreProductListFree(void *context, void *inProductList)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StoreProductListGet(void *context, void *outProductList, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StoreProductsShow(void *context, unsigned *inProductTagList)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

// Here I have no idea if pointers need or anything like that, it is an IntPtr/void* can be struct ponter or anything.
EXPORT_FUNC int UPC_StreamingCurrentUserCountryFree(void *context, void *inUtf8Country)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StreamingCurrentUserCountryGet(void *context, void *outUtf8Country, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StreamingDeviceTypeGet(void *context,void *outType, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StreamingInputGamepadTypeGet(void *context,void *outType, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StreamingInputTypeGet(void *context,void *outType, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StreamingNetworkDelayForInputGet(void *context,void *outDelayMs, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StreamingNetworkDelayForVideoGet(void *context,void *outDelayMs, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StreamingNetworkDelayRoundtripGet(void *context,void *outDelayMs, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StreamingResolutionFree(void *context,void *inResolution)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StreamingResolutionGet(void *context,void *outResolution, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_StreamingTypeGet(void *context, void *outType, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0x200;
}


EXPORT_FUNC const char *UPC_TicketGet(void *context)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    //NOTE: jwt format
    context_data *data = (context_data *)context;
    return data->config.ticket.c_str();
    return ""; //offline
    return "GBTICKETTICKETTICKETTICKETTICKET";
}

EXPORT_FUNC int UPC_TicketGet_Extended(void *context, const char **outTicket)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    *outTicket = data->config.ticket.c_str();
    return 0;
}

EXPORT_FUNC void UPC_Uninit(void)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);

}

EXPORT_FUNC int UPC_Update(void *context)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    context_data *data = (context_data *)context;
    while (!data->cb.empty()) {
        auto cb = data->cb.front();
        cb.cb(cb.arg1, cb.context_data);
        data->cb.pop();
    }

    return 0;
}

struct uplay_user {
    char *uplay_id;
    char *username;
    void *unknown;
    uint32_t *unknown_int; //set to 1
};

EXPORT_FUNC int UPC_UserFree(void *context, struct uplay_user *inUser)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    if (!context || !inUser) return -0xd;
    if (inUser->uplay_id) delete[] inUser->uplay_id;
    if (inUser->username) delete[] inUser->username;
    if (inUser->unknown_int) delete[] inUser->unknown_int;
    delete inUser;
    return 0;
}

EXPORT_FUNC int UPC_UserGet(void *context, char *inOptUserIdUtf8, struct uplay_user **outUser, void *inCallback, void *inCallbackData)
{
    PRINT_DEBUG("%s %p\n", __FUNCTION__, inOptUserIdUtf8);
    context_data *data = (context_data *)context;
    data->cb.push(cb_data(inCallback, inCallbackData, 0));

    struct uplay_user *u = new uplay_user();
    std::string uplay_id = data->config.uplay_id;
    std::string username = data->config.name;
    u->uplay_id = new char[uplay_id.size() + 1];
    uplay_id.copy(u->uplay_id, uplay_id.size());
    u->uplay_id[uplay_id.size()] = 0;

    u->username = new char[username.size() + 1];
    username.copy(u->username, username.size());
    u->username[username.size()] = 0;

    u->unknown = 0;

    u->unknown_int = new uint32_t;
    *(u->unknown_int) = 1;

    *outUser = u;
    return 0x10000;
}

EXPORT_FUNC int UPC_UserPlayedWithAdd(void *context, char *inUserIdUtf8List, int inListLength)
{
    PRINT_DEBUG("%s\n", __FUNCTION__);
    return 0;
}

EXPORT_FUNC int UPC_UserPlayedWithAdd_Extended(void *context, char *inUserIdUtf8List, int inListLength, void *unk_0, void *unk_1)
{
    PRINT_DEBUG("%s %p %p\n", __FUNCTION__, unk_0, unk_1);
    return 0;
}
