# Scene file language

```
Grammar:

  Notation:
    |        -> If/or statement
    * or +   -> while or for loop
    *N       -> loop N times
    ^X       -> anything but X
    []       -> range

  Scene         -> Statement* EOF

  Statement     -> ExprStmt
                 | ObjStmt
                 | ValStmt
                 | MacroStmt

  ExprStmt      -> Expression
  ObjStmt       -> "obj" Identifier "=" Expression
  ValStmt       -> "val" Identifier "=" Expression
  MacroStmt     -> "%" Identifier ( NUMBER | STRING )

  Expression    -> Literal | Object
  Literal       -> NUMBER | STRING

  Object        -> Vec3
                 | Texture
                 | Material
                 | Camera

  Vec3          -> "Vec3" "{" ( (NUMBER | Identifier) ";" )*3 "}"

  Texture       -> Texture:Consant
                 | Texture:Checker
                 | Texture:Noise
                 | Texture:Image

  Material      -> Material:Lambertian
                 | Material:Metal
                 | Material:Dielectric
                 | Material:DiffuseLight
                 | Material:Isotropic

  Camera        -> "Camera" "{"
                      "LookFrom" ":" ( Vec3 | Identifier ) ";"
                      "LookAt" ":" ( Vec3 | Identifier ) ";"
                      "Vfov" ":" ( NUMBER | Identifier ) ";"
                      "Aperature" ":" ( NUMBER | Identifier ) ";"
                      "Focus" ":" ( NUMBER | Identifier ) ";"
                    "}"

  T:Constant    -> "Texture" ":" "Constant" "{"
                      "Color" ":" ( Vec3 | Identifier ) ";"
                   "}"

  T:Checker     -> "Texture" ":" "Checker" "{"
                      "Even" ":" ( Texture | Identifier ) ";"
                      "Odd" ":" ( Texture | Identifier ) ";"
                   "}"

  T:Noise       -> "Texture" ":" "Noise" "{"
                      "Scale" ":" ( NUMBER | Identifier ) ";"
                      "Type" ":" ( NUMBER | Identifier ) ";"
                   "}"

  T:Image       -> "Texture" ":" "Image" "{"
                      "Path" ":" ( STRING | Identifier ) ";"
                   "}"

  M:Lambertian   -> "Material" ":" "Lambertian" "{"
                      "Albedo" ":" ( Texture | Identifier ) ";"
                    "}"

  M:Metal        -> "Material" ":" "Metal" "{"
                      "Albedo" ":" ( Texture | Identifier ) ";"
                      "Fuzz" ":" ( NUMBER | Identifier ) ";"
                    "}"

  M:Dieletric    -> "Material" ":" "Dieletric" "{"
                      "Albedo" ":" ( Texture | Identifier ) ";"
                      "Refraction" ":" ( NUMBER | Identifier ) ";"
                    "}"

  M:DiffuseLight -> "Material" ":" "DiffuseLight" "{"
                      "Emit" ":" ( Texture | Identifier ) ";"
                    "}"

  M:Isotropic    -> "Material" ":" "Isotropic" "{"
                      "Albedo" ":" ( Texture | Identifier ) ";"
                    "}"

  Identifier    -> [a-zA-Z_][a-zA-Z_0-9]*
  NUMBER        -> [0-9]*
  STRING        -> '"' (^")* '"'

```
