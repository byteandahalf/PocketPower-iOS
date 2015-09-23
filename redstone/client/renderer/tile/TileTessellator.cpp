#include "../../../../mcpe/client/renderer/tile/TileTessellator.h"
#include "../../../../mcpe/client/renderer/renderer/Tessellator.h"
#include "../../../../mcpe/client/renderer/texture/TextureUVCoordinateSet.h"
#include "../../../../mcpe/world/level/TilePos.h"
#include "../../../../mcpe/world/level/TileSource.h"
#include "../../../world/level/tile/RedstoneTile.h"
#include "../../../world/level/tile/NotGateTile.h"


bool TileTessellator::tessellateRedstoneInWorld(RedstoneTile* tile, const TilePos& pos, int data) {
	int x = pos.x, y = pos.y, z = pos.z;

	TextureUVCoordinateSet cross(0.125F, 0.5F, 0.1562F, 0.5625F);
	TextureUVCoordinateSet line(0.1875F, 0.5F, 0.2187F, 0.5625F);

	tessellator->color(tile->getColor(tile, region, x, y, z));

	bool var19 = tile->canRedstoneConnect(region, x - 1, y, z, 1) || (!Tile::solid[region->getTile(x - 1, y, z).id] && tile->canRedstoneConnect(region, x - 1, y - 1, z, -1));
	bool var20 = tile->canRedstoneConnect(region, x + 1, y, z, 3) || (!Tile::solid[region->getTile(x + 1, y, z).id] && tile->canRedstoneConnect(region, x + 1, y - 1, z, -1));
	bool var21 = tile->canRedstoneConnect(region, x, y, z - 1, 2) || (!Tile::solid[region->getTile(x, y, z - 1).id] && tile->canRedstoneConnect(region, x, y - 1, z - 1, -1));
	bool avec3d = tile->canRedstoneConnect(region, x, y, z + 1, 0) || (!Tile::solid[region->getTile(x, y, z + 1).id] && tile->canRedstoneConnect(region, x, y - 1, z + 1, -1));

	if(!Tile::solid[region->getTile(x, y + 1, z).id]) {
		if(Tile::solid[region->getTile(x - 1, y, z).id] && tile->canRedstoneConnect(region, x - 1, y + 1, z, -1))
			var19 = true;
		if(Tile::solid[region->getTile(x + 1, y, z).id] && tile->canRedstoneConnect(region, x + 1, y + 1, z, -1))
			var20 = true;
		if(Tile::solid[region->getTile(x, y, z - 1).id] && tile->canRedstoneConnect(region, x, y + 1, z - 1, -1))
			var21 = true;
		if(Tile::solid[region->getTile(x, y, z + 1).id] && tile->canRedstoneConnect(region, x, y + 1, z + 1, -1))
			avec3d = true;
	}

	float var23 = (float)(x + 0);
	float var24 = (float)(x + 1);
	float var25 = (float)(z + 0);
	float i2 = (float)(z + 1);
	int var27 = 0;

	if((var19 || var20) && !var21 && !avec3d)
		var27 = 1;
	if((var21 || avec3d) && !var20 && !var19)
		var27 = 2;
	if(var27 == 0) {
		int var28 = 0;
		int var29 = 0;
		int var30 = 16;
		int var31 = 16;

		if(!var19) var23 += 0.3125;
		if(!var19) var28 += 5;
		if(!var20) var24 -= 0.3125;
		if(!var20) var30 -= 5;
		if(!var21) var25 += 0.3125;
		if(!var21) var29 += 5;
		if(!avec3d) i2 -= 0.3125;
		if(!avec3d) var31 -= 5;

		tessellator->vertexUV(var24, y + 0.015625, i2, cross.getInterpolatedU(var30), cross.getInterpolatedV(var31));
		tessellator->vertexUV(var24, y + 0.015625, var25, cross.getInterpolatedU(var30), cross.getInterpolatedV(var29));
		tessellator->vertexUV(var23, y + 0.015625, var25, cross.getInterpolatedU(var28), cross.getInterpolatedV(var29));
		tessellator->vertexUV(var23, y + 0.015625, i2, cross.getInterpolatedU(var28), cross.getInterpolatedV(var31));
	} else if(var27 == 1) {
		tessellator->vertexUV(var24, y + 0.015625, i2, line.maxU, line.maxV);
		tessellator->vertexUV(var24, y + 0.015625, var25, line.maxU, line.minV);
		tessellator->vertexUV(var23, y + 0.015625, var25, line.minU, line.minV);
		tessellator->vertexUV(var23, y + 0.015625, i2, line.minU, line.maxV);
	} else {
		tessellator->vertexUV(var24, y + 0.015625, i2, line.maxU, line.maxV);
		tessellator->vertexUV(var24, y + 0.015625, var25, line.minU, line.maxV);
		tessellator->vertexUV(var23, y + 0.015625, var25, line.minU, line.minV);
		tessellator->vertexUV(var23, y + 0.015625, i2, line.maxU, line.minV);
	}

	if(!Tile::solid[region->getTile(x, y + 1, z).id]) {
		if(Tile::solid[region->getTile(x - 1, y, z).id] && region->getTile(x - 1, y + 1, z).id == tile->id) {
			tessellator->color(tile->getColor(tile, region, x, y, z));
			tessellator->vertexUV(x + 0.015625, ((float) (y + 1) + 0.021875), (z + 1), line.maxU, line.minV);
			tessellator->vertexUV(x + 0.015625, (y + 0), (z + 1), line.minU, line.minV);
			tessellator->vertexUV(x + 0.015625, (y + 0), (z + 0), line.minU, line.maxV);
			tessellator->vertexUV(x + 0.015625, ((float) (y + 1) + 0.021875), (z + 0), line.maxU, line.maxV);
		}
		if(Tile::solid[region->getTile(x + 1, y, z).id] && region->getTile(x + 1, y + 1, z).id == tile->id) {
			tessellator->color(tile->getColor(tile, region, x, y, z));
			tessellator->vertexUV((x + 1) - 0.015625, (y + 0), (z + 1), line.minU, line.maxV);
			tessellator->vertexUV((x + 1) - 0.015625, ((float) (y + 1) + 0.021875), (z + 1), line.maxU, line.maxV);
			tessellator->vertexUV((x + 1) - 0.015625, ((float) (y + 1) + 0.021875), (z + 0), line.maxU, line.minV);
			tessellator->vertexUV((x + 1) - 0.015625, (y + 0), (z + 0), line.minU, line.minV);
		}
		if(Tile::solid[region->getTile(x, y, z - 1).id] && region->getTile(x, y + 1, z - 1).id == tile->id) {
			tessellator->color(tile->getColor(tile, region, x, y, z));
			tessellator->vertexUV((x + 1), (y + 0), z + 0.015625, line.minU, line.maxV);
			tessellator->vertexUV((x + 1), ((float) (y + 1) + 0.021875), z + 0.015625, line.maxU, line.maxV);
			tessellator->vertexUV((x + 0), ((float) (y + 1) + 0.021875), z + 0.015625, line.maxU, line.minV);
			tessellator->vertexUV((x + 0), (y + 0), z + 0.015625, line.minU, line.minV);
		}
		if(Tile::solid[region->getTile(x, y, z + 1).id] && region->getTile(x, y + 1, z + 1).id == tile->id) {
			tessellator->color(tile->getColor(tile, region, x, y, z));
			tessellator->vertexUV((x + 1), ((float) (y + 1) + 0.021875), (z + 1) - 0.015625, line.maxU, line.minV);
			tessellator->vertexUV((x + 1), (y + 0), (z + 1) - 0.015625, line.minU, line.minV);
			tessellator->vertexUV((x + 0), (y + 0), (z + 1) - 0.015625, line.minU, line.maxV);
			tessellator->vertexUV((x + 0), ((float) (y + 1) + 0.021875), (z + 1) - 0.015625, line.maxU, line.maxV);
		}
	}
	return true;
}

bool TileTessellator::tessellateLitNotGateInWorld(NotGateTile* tile, const TilePos& pos, int data) {
	int x = pos.x, y = pos.y, z = pos.z;

	tessellator->color(0xFFFFFF);

	double var7 = 0.4000000059604645;
	double var9 = 0.5F - var7;
	double var11 = 0.20000000298023224;

	if(data == 1)
		tessellateAngledNotGate(tile, x - var9, y + var11, z, -var7, 0.0F);
	else if(data == 2)
		tessellateAngledNotGate(tile, x + var9, y + var11, z, var7, 0.0F);
	else if(data == 3)
		tessellateAngledNotGate(tile, x, y + var11, z - var9, 0.0F, -var7);
	else if(data == 4)
		tessellateAngledNotGate(tile, x, y + var11, z + var9, 0.0F, var7);
	else
		tessellateAngledNotGate(tile, x, y, z, 0.0F, 0.0F);
	return true;
}

void TileTessellator::tessellateAngledNotGate(Tile* tile, double x, double y, double z, double xRot, double zRot) {
	TextureUVCoordinateSet torch = tile->tex;

	float var15 = torch.minU;
	float var17 = torch.minV;
	float var19 = torch.maxU;
	float var21 = torch.maxV;
	float var23 = torch.getInterpolatedU(7.0F);
	float var25 = torch.getInterpolatedV(6.0F);
	float var27 = torch.getInterpolatedU(9.0F);
	float var29 = torch.getInterpolatedV(8.0F);
	float var31 = torch.getInterpolatedU(7.0F);
	float var33 = torch.getInterpolatedV(13.0F);
	float var35 = torch.getInterpolatedU(9.0F);
	float var37 = torch.getInterpolatedV(15.0F);
	x += 0.5F;
	z += 0.5F;
	float var39 = x - 0.5F;
	float var41 = x + 0.5F;
	float var43 = z - 0.5F;
	float var45 = z + 0.5F;
	float var47 = 0.0625F;
	float var49 = 0.625F;

	tessellator->vertexUV(x + xRot * (1.0F - var49) - var47, y + var49, z + zRot * (1.0F - var49) - var47, var23, var25);
	tessellator->vertexUV(x + xRot * (1.0F - var49) - var47, y + var49, z + zRot * (1.0F - var49) + var47, var23, var29);
	tessellator->vertexUV(x + xRot * (1.0F - var49) + var47, y + var49, z + zRot * (1.0F - var49) + var47, var27, var29);
	tessellator->vertexUV(x + xRot * (1.0F - var49) + var47, y + var49, z + zRot * (1.0F - var49) - var47, var27, var25);
	tessellator->vertexUV(x + var47 + xRot, y, z - var47 + zRot, var35, var33);
	tessellator->vertexUV(x + var47 + xRot, y, z + var47 + zRot, var35, var37);
	tessellator->vertexUV(x - var47 + xRot, y, z + var47 + zRot, var31, var37);
	tessellator->vertexUV(x - var47 + xRot, y, z - var47 + zRot, var31, var33);
	tessellator->vertexUV(x - var47, y + 1.0F, var43, var15, var17);
	tessellator->vertexUV(x - var47 + xRot, y + 0.0F, var43 + zRot, var15, var21);
	tessellator->vertexUV(x - var47 + xRot, y + 0.0F, var45 + zRot, var19, var21);
	tessellator->vertexUV(x - var47, y + 1.0F, var45, var19, var17);
	tessellator->vertexUV(x + var47, y + 1.0F, var45, var15, var17);
	tessellator->vertexUV(x + xRot + var47, y + 0.0F, var45 + zRot, var15, var21);
	tessellator->vertexUV(x + xRot + var47, y + 0.0F, var43 + zRot, var19, var21);
	tessellator->vertexUV(x + var47, y + 1.0F, var43, var19, var17);
	tessellator->vertexUV(var39, y + 1.0F, z + var47, var15, var17);
	tessellator->vertexUV(var39 + xRot, y + 0.0F, z + var47 + zRot, var15, var21);
	tessellator->vertexUV(var41 + xRot, y + 0.0F, z + var47 + zRot, var19, var21);
	tessellator->vertexUV(var41, y + 1.0F, z + var47, var19, var17);
	tessellator->vertexUV(var41, y + 1.0F, z - var47, var15, var17);
	tessellator->vertexUV(var41 + xRot, y + 0.0F, z - var47 + zRot, var15, var21);
	tessellator->vertexUV(var39 + xRot, y + 0.0F, z - var47 + zRot, var19, var21);
	tessellator->vertexUV(var39, y + 1.0F, z - var47, var19, var17);
}