#include "CoreMinimal.h"
#include "InsPackageType.generated.h"

UENUM()
enum class EPackageType : uint8
{
    T_REGISTER UMETA(DisplayName = "Register Data"),
    T_CLIENT_FRAME UMETA(DisplayName = "Client Frame"),          
    T_FRAME UMETA(DisplayName = "Frame Data"),                   
    T_REGISTER_RESPONSE UMETA(DisplayName = "Register Response") ,
	T_CLIENT_SYNC_POSITION UMETA(DisplayName = "Client Position Sync")
};
