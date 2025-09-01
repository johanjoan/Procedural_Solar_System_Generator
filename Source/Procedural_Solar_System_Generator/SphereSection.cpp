// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereSection.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
ASphereSection::ASphereSection()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Default scene component"));
    SetRootComponent(root);
}

// Called when the game starts or when spawned
void ASphereSection::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ASphereSection::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void ASphereSection::Initialize(int inRadius, const TArray<float>& inNoiseScale, const TArray<int32>& inHeight,
    PerlinNoise* InNoise, float InminLat, float InmaxLat, float InminLon, float InmaxLon, UMaterialInstanceDynamic* InMaterial)
{
    radius = inRadius;
    Heights = inHeight;
    NoiseScales = inNoiseScale;
    LOD = 0;
    Noise = InNoise;
    minLat = InminLat;
    maxLat = InmaxLat;
    minLon = InminLon;
    maxLon = InmaxLon;
    CustomMaterial = InMaterial;
}

void ASphereSection::CreateNewMesh(int N)
{
   
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UV0;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> VertexColors;

    const int32 LongitudeSegments = N;
    const int32 LatitudeSegments = N;

   
    //Generate Vertices in a sphere
    for (int32 LatIndex = 0; LatIndex <= LatitudeSegments; LatIndex++)
    {
        float Lat = minLat + (maxLat - minLat) * (LatIndex / (float)LatitudeSegments);

        for (int32 LonIndex = 0; LonIndex <= LongitudeSegments; LonIndex++)
        {
            float Lon = minLon + (maxLon - minLon) * (LonIndex / (float)LongitudeSegments);
            
            FVector point2 = CalculateMeshPoint(Lat, Lon);

            float alc = FVector::Dist(point2, FVector::Zero());
            if (alc > MaxHeight) MaxHeight = alc;

            Vertices.Add(point2);
            Normals.Add(CalculateNormal(point2,Lat,Lon));

            float U = (Lon + 180.0f) / 360.0f;  // Normalizate Lon from [-180, 180] to [0,1]
            float V = (Lat + 90.0f) / 180.0f;   // Normalizate Lat from [-90, 90] to [0,1]

            UV0.Add(FVector2D(U, V));
        }
    }
    // Generate triangles
    for (int32 Lat = 0; Lat < LatitudeSegments; ++Lat)
    {
        for (int32 Lon = 0; Lon < LongitudeSegments; ++Lon)
        {
            int32 Current = Lon + Lat * (LongitudeSegments + 1);
            int32 Next = Current + LongitudeSegments + 1;

            // Triangle 1 
            Triangles.Add(Current);
            Triangles.Add(Next);
            Triangles.Add(Current + 1);

            // Triangle 2 
            Triangles.Add(Current + 1);
            Triangles.Add(Next);
            Triangles.Add(Next + 1);
        }
    }

    UProceduralMeshComponent* NewMesh = NewObject<UProceduralMeshComponent>(this);
    NewMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
    
    NewMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    
    if (MyMeshes.IsEmpty()) NewMesh->SetHiddenInGame(false); //Hide all meshes except the first one initialy
    else  NewMesh->SetHiddenInGame(true);

    NewMesh->bCastDynamicShadow = true;
    NewMesh->SetCastShadow(true);
    NewMesh->bAffectDistanceFieldLighting = true;
    NewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    NewMesh->RegisterComponent();
    NewMesh->SetMaterial(0, CustomMaterial);
    MyMeshes.Emplace(NewMesh);
}

void ASphereSection::UpdateLOD(int InLOD)
{

    if (InLOD == LOD) return;

    ////Hide current mesh
    MyMeshes[LOD]->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
    MyMeshes[LOD]->SetHiddenInGame(true);
    MyMeshes[LOD]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MyMeshes[LOD]->SetVisibility(false);
    
    //Show new mesh
    LOD = InLOD;
    MyMesh = MyMeshes[LOD];
    MyMeshes[LOD]->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    MyMeshes[LOD]->SetHiddenInGame(false);
    MyMeshes[LOD]->SetVisibility(true);
    MyMeshes[LOD]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    // If the LOD is the higher one (More detailed) we manage the decorations of the section
    if (LOD == (MyMeshes.Num()-1))
    {
        for (int32 i = 0; i < ArrayInstancedDecoracions.Num(); i++) {
            ArrayInstancedDecoracions[i]->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            ArrayInstancedDecoracions[i]->SetVisibility(true);
            ArrayInstancedDecoracions[i]->SetRelativeTransform(SavedRelativeTransforms[i]);
        }
    }
    else
    {
         for (int32 i = 0; i < ArrayInstancedDecoracions.Num(); i++) {
            ArrayInstancedDecoracions[i]->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            ArrayInstancedDecoracions[i]->SetVisibility(false);
         }
    }
}

void ASphereSection::ActivateDecorationCollisions()
{
    for (int32 i = 0; i < ArrayInstancedDecoracions.Num(); i++) {
        ArrayInstancedDecoracions[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
}

void ASphereSection::DeactivateDecorationCollisions()
{
    for (int32 i = 0; i < ArrayInstancedDecoracions.Num(); i++) {
        ArrayInstancedDecoracions[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

float ASphereSection::getMinLat()
{
    return minLat;
}

float ASphereSection::getMinLon()
{
    return minLon;
}

float ASphereSection::getMaxLat()
{
    return maxLat;
}

float ASphereSection::getMaxLon()
{
    return maxLon;
}



FVector ASphereSection::CalculateNormal(const FVector& point, float lat, float lon)
{
    float epsilon = 0.000001f * radius;

    FVector P_theta_plus = CalculateMeshPoint(lat + epsilon, lon);
    FVector P_theta_minus = CalculateMeshPoint(lat - epsilon, lon);
    FVector P_phi_plus = CalculateMeshPoint(lat, lon + epsilon);
    FVector P_phi_minus = CalculateMeshPoint(lat, lon - epsilon);

    FVector T_theta = (P_theta_plus - P_theta_minus) / (2 * epsilon);
    FVector T_phi = (P_phi_plus - P_phi_minus) / (2 * epsilon);

    FVector Normal = FVector::CrossProduct(T_theta, T_phi).GetSafeNormal();

    if (FVector::DotProduct(Normal, point) < 0)
    {
        Normal = -Normal;
    }

    return Normal;
}


//Point on a sphere of radius = 1
FVector ASphereSection::CalculatePointFromLatitudeLongitude(float latitude, float longitude)
{
    float theta = FMath::DegreesToRadians(90.0f - latitude);
    float phi = FMath::DegreesToRadians(longitude);

    return FVector(
        FMath::Sin(theta) * FMath::Cos(phi),
        FMath::Sin(theta) * FMath::Sin(phi),
        FMath::Cos(theta)
    );
}


bool ASphereSection::isSectioninViewPort()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController || !MyMesh) return false;

    int32 ViewportX, ViewportY;
    PlayerController->GetViewportSize(ViewportX, ViewportY);

    // Access the section of a mesh with low resolution
    const FProcMeshSection* MeshSection = MyMeshes[1]->GetProcMeshSection(0);
    if (!MeshSection) return false;

    const TArray<FProcMeshVertex>& Vertices = MeshSection->ProcVertexBuffer;

    for (const FProcMeshVertex& Vertex : Vertices)
    {
        // Translate local vertex to real World
        FVector WorldPoint = MyMesh->GetComponentTransform().TransformPosition(Vertex.Position);

        // Project to screen
        FVector2D ScreenPos;
        if (PlayerController->ProjectWorldLocationToScreen(WorldPoint, ScreenPos))
        {
            // Check if it is inside the screen with a certain margin
            float MargeX = ViewportX * 0.15f;
            float MargeY = ViewportY * 0.15f;

            if (ScreenPos.X >= 0 && ScreenPos.X <= ViewportX &&
                ScreenPos.Y >= 0 && ScreenPos.Y <= ViewportY)
            {
                return true; 
            }
        }
    }
    return false;
}


void ASphereSection::addActor(float lat, float lon, UStaticMesh* Mesh,FVector Scale)
{
    FTransform Actor;

    FVector PosActor = CalculateMeshPoint(lat, lon);


    FVector normal = CalculateNormal(PosActor, lat, lon);
    FVector normalized = normal.GetSafeNormal();

    // Rotation alligned with Normal Vector
    FRotator NewRotation = FRotationMatrix::MakeFromZ(normalized).Rotator();

    FVector PosSection = GetActorLocation();
    FVector v = (PosSection - PosActor).GetSafeNormal();
    Actor.SetLocation(PosActor-normalized*75);
    Actor.SetRotation(NewRotation.Quaternion());
    Actor.SetScale3D(Scale);

    ArrayInstancedDecoracions.Last()->AddInstance(Actor);
}



FVector ASphereSection::CalculateMeshPoint(float Lat, float Lon) {
    FVector point = CalculatePointFromLatitudeLongitude(Lat, Lon);

    float noise = 0;

    for (int i = 0; i < NoiseScales.Num(); i++) {
        float addNoise = (Noise->perlin3D(point.X * NoiseScales[i], point.Y * NoiseScales[i], point.Z * NoiseScales[i]) + 1)/2.0f ;
        noise += addNoise* Heights[i];
    }
    FVector point2 = point * (radius + noise);
    return point2;
}



void ASphereSection::AddDecoration(UStaticMesh* Mesh)
{
    UHierarchicalInstancedStaticMeshComponent* NewDecoration = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);

    NewDecoration->SetupAttachment(RootComponent);
    NewDecoration->SetMobility(EComponentMobility::Movable);
    NewDecoration->SetVisibility(false);
    NewDecoration->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    NewDecoration->SetStaticMesh(Mesh);

    NewDecoration->RegisterComponent();
    SavedRelativeTransforms.Add(NewDecoration->GetRelativeTransform());
    ArrayInstancedDecoracions.Add(NewDecoration);
}

float ASphereSection::getMaxHeight()
{
    return MaxHeight;
}

void ASphereSection::CleanDecorations()
{
    for (UHierarchicalInstancedStaticMeshComponent* Decoration : ArrayInstancedDecoracions)
    {
        if (Decoration)
        {
            Decoration->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            Decoration->DestroyComponent();
        }
    }

    ArrayInstancedDecoracions.Empty();
    SavedRelativeTransforms.Empty(); 
}
