enum UPC_Result
{
  UPC_Result_Ok = 0x0,
  UPC_Result_Declined = 0xFFFFFFFF,
  UPC_Result_InvalidArgs = 0xFFFFFFFE,
  UPC_Result_UninitializedSubsystem = 0xFFFFFFFD,
  UPC_Result_CommunicationError = 0xFFFFFFFC,
  UPC_Result_MemoryError = 0xFFFFFFFB,
  UPC_Result_NotFound = 0xFFFFFFFA,
  UPC_Result_EOF = 0xFFFFFFF9,
  UPC_Result_LimitReached = 0xFFFFFFF8,
  UPC_Result_UnauthorizedAction = 0xFFFFFFF7,
  UPC_Result_InternalError = 0xFFFFFFF6,
  UPC_Result_Aborted = 0xFFFFFFF5,
  UPC_Result_Unknown = 0xFFFFFFF4,
  UPC_Result_FailedPrecondition = 0xFFFFFFF3,
  UPC_Result_Unavailable = 0xFFFFFFF2,
};

enum UPC_InitResult : int
{
  UPC_InitResult_Ok = 0x0,
  UPC_InitResult_Failed = 0x1,
  UPC_InitResult_IncompatibleApiVersion = 0x2,
  UPC_InitResult_ExitProcessRequired = 0x3,
  UPC_InitResult_InstallationError = 0x4,
  UPC_InitResult_DesktopInteractionRequired = 0x5,
  UPC_InitResult_AlreadyInitialized = 0x6,
};

enum UPC_OverlaySection : int
{
  UPC_OverlaySection_None = 0x0,
  UPC_OverlaySection_Home = 0x1,
  UPC_OverlaySection_Achievements = 0x2,
  UPC_OverlaySection_Actions = 0x3,
  UPC_OverlaySection_Chat = 0x4,
  UPC_OverlaySection_Friends = 0x5,
  UPC_OverlaySection_Party = 0x6,
  UPC_OverlaySection_Rewards = 0x7,
  UPC_OverlaySection_Shop = 0x8,
  UPC_OverlaySection_ProductActivation = 0x9,
  UPC_OverlaySection_PendingGameInvites = 0xA,
  UPC_OverlaySection_Challenges = 0xB,
  UPC_OverlaySection_GameOptions = 0xC,
};

enum UPC_ContextSubsystem : int
{
  UPC_ContextSubsystem_None = 0x0,
  UPC_ContextSubsystem_Achievement = 0x1,
  UPC_ContextSubsystem_Product = 0x2,
  UPC_ContextSubsystem_Install = 0x4,
  UPC_ContextSubsystem_Storage = 0x8,
  UPC_ContextSubsystem_Overlay = 0x10,
  UPC_ContextSubsystem_Friend = 0x20,
  UPC_ContextSubsystem_Multiplayer = 0x40,
  UPC_ContextSubsystem_Store = 0x80,
  UPC_ContextSubsystem_Streaming = 0x100,
};

enum UPC_ProductType : int
{
  UPC_ProductType_Game = 0x1,
  UPC_ProductType_Addon = 0x2,
  UPC_ProductType_Package = 0x3,
  UPC_ProductType_Consumable = 0x4,
  UPC_ProductType_ConsumablePack = 0x5,
  UPC_ProductType_Bundle = 0x6,
};

enum UPC_ProductState : int
{
  UPC_ProductState_Visible = 0x1,
  UPC_ProductState_Installable = 0x2,
  UPC_ProductState_Playable = 0x3,
  UPC_ProductState_Expired = 0x4,
};

enum UPC_ProductOwnership : int
{
  UPC_ProductOwnership_Owned = 0x1,
  UPC_ProductOwnership_Preordered = 0x2,
  UPC_ProductOwnership_Suspended = 0x3,
  UPC_ProductOwnership_Revoked = 0x4,
  UPC_ProductOwnership_NotOwned = 0x4,
  UPC_ProductOwnership_Locked = 0x5,
};

enum UPC_ProductActivation : int
{
  UPC_ProductActivation_Purchased = 0x1,
  UPC_ProductActivation_Trial = 0x2,
  UPC_ProductActivation_Subscription = 0x3,
};

enum UPC_FileOpenMode : int
{
  UPC_FileOpenMode_Read = 0x1,
  UPC_FileOpenMode_Write = 0x2,
};

enum UPC_EventType : int
{
  UPC_Event_FriendAdded = 0x0,
  UPC_Event_FriendNameUpdated = 0x1,
  UPC_Event_FriendPresenceUpdated = 0x2,
  UPC_Event_FriendRemoved = 0x3,
  UPC_Event_MultiplayerSessionCleared = 0x3E8,
  UPC_Event_MultiplayerSessionUpdated = 0x3E9,
  UPC_Event_MultiplayerInviteReceived = 0x3EA,
  UPC_Event_MultiplayerInviteAccepted = 0x3EB,
  UPC_Event_MultiplayerInviteDeclined = 0x3EC,
  UPC_Event_MultiplayerJoiningRequested = 0x3ED,
  UPC_Event_OverlayShown = 0x7D0,
  UPC_Event_OverlayHidden = 0x7D1,
  UPC_Event_ProductAdded = 0xBB8,
  UPC_Event_ProductOwnershipUpdated = 0xBB9,
  UPC_Event_ProductStateUpdated = 0xBBA,
  UPC_Event_ProductBalanceUpdated = 0xBBB,
  UPC_Event_InstallChunkInstalled = 0xFA0,
  UPC_Event_InstallChunkProgress = 0xFA1,
  UPC_Event_InstallProgress = 0xFA2,
  UPC_Event_UpdateAvailable = 0xFA3,
  UPC_Event_StoreProductsListUpdated = 0x1388,
  UPC_Event_StoreStatusUpdated = 0x1389,
  UPC_Event_StoreCheckoutStarted = 0x138A,
  UPC_Event_StoreCheckoutFinished = 0x138B,
  UPC_Event_UserShutdown = 0x1770,
  UPC_Event_StreamingCurrentUserCountryUpdated = 0x1B58,
  UPC_Event_StreamingDeviceUpdated = 0x1B59,
  UPC_Event_StreamingInputTypeUpdated = 0x1B5A,
  UPC_Event_StreamingResolutionUpdated = 0x1B5B,
};

const struct __declspec(align(8)) UPC_Achievement
{
  unsigned int id;
  const char *nameUtf8;
  const char *descriptionUtf8;
  int completed;
};

struct UPC_AchievementList
{
  unsigned int count;
  UPC_Achievement **list;
};

const struct UPC_Product
{
  unsigned int id;
  unsigned int type;
  unsigned int ownership;
  unsigned int state;
  unsigned int balance;
  unsigned int activation;

  UPC_Product(uint32_t _id, uint32_t _type)
  {
     id = _id;
     type = _type;
     ownership = UPC_ProductOwnership_Owned;
     state = UPC_ProductState_Playable;
     balance = 0;
     activation = UPC_ProductActivation_Purchased;
  }
};

struct UPC_ProductList
{
  unsigned int count;
  UPC_Product **list;
};

struct __declspec(align(8)) UPC_Presence
{
  unsigned int onlineStatus;
  char *detailsUtf8;
  unsigned int titleId;
  char *titleNameUtf8;
  char *multiplayerId;
  int multiplayerJoinable;
  unsigned int multiplayerSize;
  unsigned int multiplayerMaxSize;
  void *multiplayerInternalData;
  unsigned int multiplayerInternalDataSize;
};

struct UPC_User
{
  char *idUtf8;
  char *nameUtf8;
  unsigned int relationship;
  UPC_Presence *presence;
};

struct UPC_EventData_FriendAdded
{
  UPC_User newFriend;
};

struct UPC_EventData_FriendNameUpdated
{
  char *friendId;
  char *newName;
};

struct UPC_EventData_FriendPresenceUpdated
{
  char *friendId;
  UPC_Presence newPresence;
};

struct UPC_EventData_FriendRemoved
{
  char *friendId;
};

struct __declspec(align(8)) UPC_MultiplayerSession
{
  char *id;
  unsigned int joinability;
  unsigned int size;
  unsigned int maxSize;
  void *internalData;
  unsigned int internalDataSize;
};

struct UPC_EventData_MultiplayerSessionUpdated
{
  UPC_MultiplayerSession newSession;
};

struct UPC_EventData_MultiplayerInviteReceived
{
  char *senderId;
  UPC_MultiplayerSession session;
};

struct UPC_EventData_MultiplayerInviteAccepted
{
  char *senderId;
  UPC_MultiplayerSession session;
};

struct UPC_EventData_MultiplayerInviteDeclined
{
  char *senderId;
};

struct UPC_EventData_MultiplayerJoiningRequested
{
  UPC_MultiplayerSession session;
};

struct UPC_EventData_ProductAdded
{
  UPC_Product product;
};

struct UPC_EventData_ProductOwnershipUpdated
{
  unsigned int id;
  unsigned int newOwnership;
};

struct UPC_EventData_ProductStateUpdated
{
  unsigned int id;
  unsigned int newState;
};

struct UPC_EventData_ProductBalanceUpdated
{
  unsigned int id;
  unsigned int newBalance;
};

struct UPC_EventData_InstallChunkInstalled
{
  unsigned int id;
};

struct UPC_EventData_InstallChunkProgress
{
  unsigned int id;
  unsigned long long installedBytes;
  unsigned long long sizeInBytes;
};

struct UPC_EventData_InstallProgress
{
  unsigned long long installedBytes;
  unsigned long long sizeInBytes;
  unsigned long long bytesPerSecond;
};

struct UPC_EventData_StoreCheckoutStarted
{
  unsigned int productId;
};

struct UPC_EventData_StoreCheckoutFinished
{
  unsigned int productId;
  int result;
};

struct UPC_EventData_StoreStatusUpdated
{
  int enabled;
};

struct UPC_EventData_UserShutdown
{
  unsigned int reason;
};

union UPC_EventData
{
  UPC_EventData_FriendAdded friendAdded;
  UPC_EventData_FriendNameUpdated friendNameUpdated;
  UPC_EventData_FriendPresenceUpdated friendPresenceUpdated;
  UPC_EventData_FriendRemoved friendRemoved;
  UPC_EventData_MultiplayerSessionUpdated sessionUpdated;
  UPC_EventData_MultiplayerInviteReceived inviteReceived;
  UPC_EventData_MultiplayerInviteAccepted inviteAccepted;
  UPC_EventData_MultiplayerInviteDeclined inviteDeclined;
  UPC_EventData_MultiplayerJoiningRequested joiningRequested;
  UPC_EventData_ProductAdded productAdded;
  UPC_EventData_ProductOwnershipUpdated productOwnershipUpdated;
  UPC_EventData_ProductStateUpdated productStateUpdated;
  UPC_EventData_ProductBalanceUpdated productBalanceUpdated;
  UPC_EventData_InstallChunkInstalled installChunkInstalled;
  UPC_EventData_InstallChunkProgress installChunkProgress;
  UPC_EventData_InstallProgress installProgress;
  UPC_EventData_StoreCheckoutStarted storeCheckoutStarted;
  UPC_EventData_StoreCheckoutFinished storeCheckoutFinished;
  UPC_EventData_StoreStatusUpdated storeStatusUpdated;
  UPC_EventData_UserShutdown userShutdown;
};

struct UPC_Event
{
  unsigned int type;
  UPC_EventData data;
};

struct UPC_Overlay_MicroAppParam
{
  const char *paramNameUtf8;
  const char *paramValueUtf8;
};

struct UPC_Overlay_MicroAppParamList
{
  unsigned int count;
  const UPC_Overlay_MicroAppParam **list;
};

struct UPC_ContextSettings
{
  unsigned __int64 subsystems;
};

struct UPC_StorageFile
{
  char *fileNameUtf8;
  char *legacyNameUtf8;
  unsigned int size;
  unsigned long long lastModifiedMs;
};

struct UPC_RichPresenceToken
{
  char *idUtf8;
  char *valueIdUtf8;
};

struct UPC_RichPresenceTokenList
{
  unsigned int count;
  UPC_RichPresenceToken **list;
};

struct UPC_StorageFileList
{
  unsigned int count;
  UPC_StorageFile **list;
};

struct UPC_StoreTagList
{
  unsigned int count;
  unsigned int *list;
};

struct UPC_StoreProduct
{
  unsigned int id;
  char *titleUtf8;
  char *descriptionUtf8;
  char *imageUrlUtf8;
  unsigned char isOwned;
  float price;
  float priceOriginal;
  char *currencyUtf8;
  char *userBlobUtf8;
  UPC_StoreTagList tags;
};

struct UPC_StoreProductList
{
  unsigned int count;
  UPC_StoreProduct **list;
};

struct UPC_InstallChunkList
{
  unsigned int count;
  unsigned int *list;
};

typedef void (__fastcall *UplayEventHandler)(UPC_Event *, void *);
typedef void (__fastcall *UPC_Callback)(int, void *);
typedef void (__fastcall *UPC_EventHandler)(UPC_Event *, void *);