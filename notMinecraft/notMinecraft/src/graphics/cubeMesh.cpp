#include "cubeMesh.h"

static const int texAtlas_Rows = 4;
static const int texAtlas_Cols = 4;

float verts[] = {
    // back face
    -0.5f, -0.5f, -0.5f, //0
     0.5f, -0.5f, -0.5f, //1
     0.5f,  0.5f, -0.5f, //2
    -0.5f,  0.5f, -0.5f, //3
 //
    // front face
    -0.5f, -0.5f,  0.5f, //4
     0.5f, -0.5f,  0.5f, //5
     0.5f,  0.5f,  0.5f, //6
    -0.5f,  0.5f,  0.5f, //7

    // left face
    -0.5f, -0.5f, -0.5f, //8
    -0.5f, -0.5f,  0.5f, //9
    -0.5f,  0.5f,  0.5f, //10
    -0.5f,  0.5f, -0.5f, //11

    // right face
     0.5f, -0.5f, -0.5f, //12
     0.5f, -0.5f,  0.5f, //13
     0.5f,  0.5f,  0.5f, //14
     0.5f,  0.5f, -0.5f, //15

     // bottom face
     -0.5f, -0.5f, -0.5f, //16
      0.5f, -0.5f, -0.5f, //17
      0.5f, -0.5f,  0.5f, //18
     -0.5f, -0.5f,  0.5f, //19

     // top face
     -0.5f,  0.5f, -0.5f, //20
      0.5f,  0.5f, -0.5f, //21
      0.5f,  0.5f,  0.5f, //22
     -0.5f,  0.5f,  0.5f  //23
};


float texCoords[] = {
    // back face
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,

    // front face
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,

    // left face
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,

    // right face
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,

    // bottom face
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,

    // top face
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};


// counter clockwise winding
unsigned int indices[] = {
   0, 3, 2, 2, 1, 0,       // back face
   4, 5, 6, 6, 7, 4,       // front face
   8, 9, 10, 10, 11, 8,    // left face
   12, 15, 14, 14, 13, 12, // right face
   16, 17, 18, 18, 19, 16, // bottom face
   22, 21, 20, 20, 23, 22  // top face
};



// todo: optimize
static float texAtlasCoord[48] = { 0.0f };
// generate tex coords from a given frameIndex
void cubeMesh::generateTexCoords(const blockTextures& textures)
{
    // convert frameindex to <x,y> whr bottom left is <0,0> top right is <cols-1, rows-1>
    int xTop = textures.frameIndex_top % texAtlas_Rows;
    int yTop = textures.frameIndex_top / texAtlas_Cols;    
    
    int xBott = textures.frameIndex_bottom % texAtlas_Rows;
    int yBott = textures.frameIndex_bottom / texAtlas_Cols;   
    
    int xSide = textures.frameIndex_sides % texAtlas_Rows;
    int ySide = textures.frameIndex_sides / texAtlas_Cols;

    // size of an individual texture in the atlas u x v
    float texSize_u = 1.0f / texAtlas_Cols;
    float texSize_v = 1.0f / texAtlas_Rows;

    // sides: 0-31
    glm::vec2 blc = { xSide * texSize_u, ySide * texSize_v };
    glm::vec2 brc = { blc.x + texSize_u, blc.y }; 
    glm::vec2 tlc = { blc.x, blc.y + texSize_v }; 
    glm::vec2 trc = { tlc.x + texSize_u, tlc.y };
    for (int i = 0; i < 4; i++)
    {
        texAtlasCoord[i * 8 + 0] = blc.x;
        texAtlasCoord[i * 8 + 1] = blc.y;
        texAtlasCoord[i * 8 + 2] = brc.x;
        texAtlasCoord[i * 8 + 3] = brc.y;
        texAtlasCoord[i * 8 + 4] = trc.x;
        texAtlasCoord[i * 8 + 5] = trc.y;
        texAtlasCoord[i * 8 + 6] = tlc.x;
        texAtlasCoord[i * 8 + 7] = tlc.y;
    }

    // bottom: 32-39
    blc = { xBott * texSize_u, yBott * texSize_v };
    brc = { blc.x + texSize_u, blc.y };
    tlc = { blc.x, blc.y + texSize_v };
    trc = { tlc.x + texSize_u, tlc.y };
    texAtlasCoord[32] = blc.x;
    texAtlasCoord[33] = blc.y;
    texAtlasCoord[34] = brc.x;
    texAtlasCoord[35] = brc.y;
    texAtlasCoord[36] = trc.x;
    texAtlasCoord[37] = trc.y;
    texAtlasCoord[38] = tlc.x;
    texAtlasCoord[39] = tlc.y;

    // top: 40-47
    blc = { xTop * texSize_u, yTop * texSize_v };
    brc = { blc.x + texSize_u, blc.y };
    tlc = { blc.x, blc.y + texSize_v };
    trc = { tlc.x + texSize_u, tlc.y };
    texAtlasCoord[40] = blc.x;
    texAtlasCoord[41] = blc.y;
    texAtlasCoord[42] = brc.x;
    texAtlasCoord[43] = brc.y;
    texAtlasCoord[44] = trc.x;
    texAtlasCoord[45] = trc.y;
    texAtlasCoord[46] = tlc.x;
    texAtlasCoord[47] = tlc.y;
}

// default
cubeMesh::cubeMesh()
{
    glGenVertexArrays(1, &vaoID);

    glGenBuffers(1, &vertsVBO);
    glGenBuffers(1, &texCoordsVBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, vertsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// with texture atlas
cubeMesh::cubeMesh(std::string type)
{
    // retrieve frameindexes, generate tex coord array 
    auto& blockFID3 = blockTypes.at(type);
    generateTexCoords(blockFID3);

    glGenVertexArrays(1, &vaoID);

    glGenBuffers(1, &vertsVBO);
    glGenBuffers(1, &texCoordsVBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, vertsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texAtlasCoord), texAtlasCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

cubeMesh::~cubeMesh()
{
}
unsigned int cubeMesh::getVAO()
{
    return vaoID;
}

GLsizei cubeMesh::numVerts()
{
    return 36;
}

