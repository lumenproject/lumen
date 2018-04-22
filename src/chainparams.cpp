// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018-2018 The Lumen Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"
#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include <math.h>
#include <stdio.h>
#include "chainparams.h"
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include <assert.h>
#include <inttypes.h>
#include <boost/assign/list_of.hpp>
#include "chainparamsseeds.h"



static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}


static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "13/Apr/2018 Coindesk - Yahoo Japan Buys Stake in Cryptocurrency Exchange";
    const CScript genesisOutputScript = CScript() << ParseHex("04551ddc3d32a7196aba3c0e11347d9bdc1bd65a8f1b2048f621030901104080e25330adc8f620a4c47c89c6a1093c6deaceede3142a48dcb1e3f48526897387135") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 350400; // one year
        consensus.nMasternodePaymentsStartBlock = 300; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 2000000000; // not used
        consensus.nMasternodePaymentsIncreasePeriod = 2000000000; // not used
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 2000000000; // not used
        consensus.nBudgetPaymentsCycleBlocks = 16616;
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockStartBlock = 2000000000; // not used
        consensus.nSuperblockCycle = 16616;
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x000005406efba08cf2147a81ef7b68f164d229b3049f0149fa8dbb19e65de886");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 6 * 60; // Lumen: 6 minutes
        consensus.nPowTargetSpacing = 90; // Lumen: 90 seconds
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1502280000; // Aug 9th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533816000; // Aug 9th, 2018

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xa2;
        pchMessageStart[1] = 0xc4;
        pchMessageStart[2] = 0xcb;
        pchMessageStart[3] = 0x4a;
        vAlertPubKey = ParseHex("04cf849725f1188b846672600b64c04141a5bff73cb2ac7aab0ab0bb4facf440259794eb7faf51ebc991f34edc8eb3ef585cda2727e8da58e152e012683861c4e4");
        nDefaultPort = 13444;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1524416400, 2053552, 0x1e0ffff0, 1, 100 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();	
        assert(consensus.hashGenesisBlock == uint256S("0x000005406efba08cf2147a81ef7b68f164d229b3049f0149fa8dbb19e65de886"));
        assert(genesis.hashMerkleRoot == uint256S("0x93c93ff59cc91e3cf834b1b8f9110389ecb4b0a983287158bc188ee3b813f646"));


//        vFixedSeeds.clear();
//        vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("seed1", "seed1.lumen.ooo"));
        vSeeds.push_back(CDNSSeedData("seed2", "seed2.lumen.ooo"));
        vSeeds.push_back(CDNSSeedData("seed3", "seed3.lumen.ooo"));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,48);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,10);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,198);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x05).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "04da9f6a07e028110685f38317ac621a574af6ddb2efb537e88bf7710f9442bd4e1c67b6a1087b19c6d18263186b933d21e399836f764e035714ec7419ce45f0fc";
        strMasternodePaymentsPubKey = "04da9f6a07e028110685f38317ac621a574af6ddb2efb537e88bf7710f9442bd4e1c67b6a1087b19c6d18263186b933d21e399836f764e035714ec7419ce45f0fc";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (    0, uint256S("0x000005406efba08cf2147a81ef7b68f164d229b3049f0149fa8dbb19e65de886")),
            1524416400, // * UNIX timestamp of last checkpoint block
            0,    //  total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            2800        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 350400;
        consensus.nMasternodePaymentsStartBlock = 300; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 2000000000;
        consensus.nMasternodePaymentsIncreasePeriod = 2000000000;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 2000000000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 2000000000; 
        consensus.nSuperblockCycle = 24;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00000aa41cc4a995c084f842bd42d254d510d48a855eee057295ad1775a91294");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 6 * 60; // Lumen: 6 minutes
        consensus.nPowTargetSpacing = 90; // Lumen: 90 seconds
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1502280000; // Aug 9th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533816000; // Aug 9th, 2018

        pchMessageStart[0] = 0x2a;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0x6c;
        vAlertPubKey = ParseHex("048a68a2121e65fb413ed219c048304d4266dec8210ba19d5101384ef1e09c2b16998e0d96ac612d2830a631fec5909ac8bc1fc779b041affbcd6cb5e58d78bbbe");
        nDefaultPort = 13555;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1524416402, 261695, 0x1e0ffff0, 1, 100 * COIN);
		
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000aa41cc4a995c084f842bd42d254d510d48a855eee057295ad1775a91294"));
        assert(genesis.hashMerkleRoot == uint256S("0x93c93ff59cc91e3cf834b1b8f9110389ecb4b0a983287158bc188ee3b813f646"));

        vFixedSeeds.clear();
        vSeeds.clear();
//      vSeeds.push_back(CDNSSeedData("tseed1",  "tseed1.lumen.ooo"));
//      vSeeds.push_back(CDNSSeedData("tseed2",  "tseed2.lumen.ooo"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,20);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,240);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "047069bedf104c686058783c01bd41f6edc3f7676b980930a6895dca284fe855535ad09c75c6ba5491de387b38e0af6ccfc872cf0129d58bb9eebc311b72e851cb";
        strMasternodePaymentsPubKey = "047069bedf104c686058783c01bd41f6edc3f7676b980930a6895dca284fe855535ad09c75c6ba5491de387b38e0af6ccfc872cf0129d58bb9eebc311b72e851cb";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x00000aa41cc4a995c084f842bd42d254d510d48a855eee057295ad1775a91294")),
	     1524416402, // * UNIX timestamp of last checkpoint block
            0,     // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            500         // * estimated number of transactions per day after checkpoint
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 7 * 60; // Lumen: 6 minutes
        consensus.nPowTargetSpacing = 90; // Lumen: 90 seconds
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        pchMessageStart[0] = 0xc1;
        pchMessageStart[1] = 0x2a;
        pchMessageStart[2] = 0xd1;
        pchMessageStart[3] = 0xab;
        nMaxTipAge = 30 * 24 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDefaultPort = 13666;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1524416404, 0, 0x207fffff, 1, 100 * COIN);
		
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x051b64d5f826cce634ea0b2265375a6b28ef18e4298dec2527546604171d3618"));
        assert(genesis.hashMerkleRoot == uint256S("0x93c93ff59cc91e3cf834b1b8f9110389ecb4b0a983287158bc188ee3b813f646"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x051b64d5f826cce634ea0b2265375a6b28ef18e4298dec2527546604171d3618")),
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,20);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,240);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();
   }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
