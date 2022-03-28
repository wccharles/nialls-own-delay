#pragma once

namespace ModDelay
{
    namespace FDNMatrixData
    {
        static constexpr auto fdnSize = 16;
        static float          delayTimes[] = { 1777.0f, 2221.0f, 3331.0f, 4111.0f };
        static float          lfoRates[] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f };
        static float          exponentialDelayTimes[] = { 0.01f, 0.0128083f, 0.0164052f, 0.0210122f, 0.026913f, 0.034471f, 0.0441513f, 0.0565503f, 0.0724311f, 0.0927718f, 0.118825f, 0.152194f, 0.194934f, 0.249677f, 0.319793f, 0.4096f };
        static float          primeDelayTimes[] = { 0.0007f, 0.0013f, 0.0023f, 0.0037f, 0.0047f, 0.0061f, 0.0079f, 0.0101f, 0.0113f, 0.0149f, 0.0173f, 0.0197f, 0.0229f, 0.0263f, 0.0293f, 0.0337f, 0.0379f, 0.0421f, 0.0461f, 0.0503f, 0.0569f, 0.0613f, 0.0659f, 0.0719f, 0.0773f, 0.0839f, 0.0907f, 0.0977f, 0.1039f, 0.1103f, 0.1187f, 0.1277f, 0.1327f, 0.1439f, 0.1511f, 0.1601f, 0.1693f, 0.1783f, 0.1879f, 0.1997f, 0.2087f, 0.2207f, 0.2309f, 0.2399f, 0.2539f, 0.2663f, 0.2749f, 0.2861f, 0.3011f, 0.3167f, 0.3301f, 0.3413f, 0.3547f, 0.3677f, 0.3823f, 0.3947f, 0.4111f, 0.4259f, 0.4423f, 0.4591f, 0.4751f, 0.4933f, 0.5077f, 0.5261f };

        static float fdnMatrix4[] = { 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f };

        static float fdnMatrix16[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f };
        //    static float my_fdnMatrix2[16][16] = { {*fdnMatrix[0], -(*fdnMatrix[0]), -(*fdnMatrix[0]), -(*fdnMatrix[0])}, {*fdnMatrix[1], -(*fdnMatrix[1]), -(*fdnMatrix[1]), -(*fdnMatrix[1])}, {*fdnMatrix[2], -(*fdnMatrix[2]), -(*fdnMatrix[2]), -(*fdnMatrix[2])}, {fdnMatrix[3], -fdnMatrix[3], -fdnMatrix[3], -fdnMatrix[3]},{-fdnMatrix[0], fdnMatrix[0], -fdnMatrix[0], -fdnMatrix[0]}, {-fdnMatrix[1], fdnMatrix[1], -fdnMatrix[1], -fdnMatrix[1]}, {-fdnMatrix[2], fdnMatrix[2], -fdnMatrix[2], -fdnMatrix[2]}, {-fdnMatrix[3], fdnMatrix[3], -fdnMatrix[3], -fdnMatrix[3]},{fdnMatrix[0], -fdnMatrix[0], -fdnMatrix[0], -fdnMatrix[0]}, {fdnMatrix[1], -fdnMatrix[1], -fdnMatrix[1], -fdnMatrix[1]}, {fdnMatrix[2], -fdnMatrix[2], -fdnMatrix[2], -fdnMatrix[2]}, {fdnMatrix[3], -fdnMatrix[3], -fdnMatrix[3], -fdnMatrix[3]}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {} };
    }
}
