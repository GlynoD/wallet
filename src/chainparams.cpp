// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018 The GLYNO Coin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h" 

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 31968473 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
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
    const char* pszTimestamp = "2018 - June: Glyno Coin is born.";
    const CScript genesisOutputScript = CScript() << ParseHex("046949dc3958ba2a1810f125ed3a650dc061e2696b5d8c01b7bdf1ec836528e86b1307ceab353c754c46266e290f9e74a4e19fe47256c2b103b5fc48b4715dad0f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}


/*
 * Main network
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 255500; // Block rewards are cut in half every 255500 blocks (about 1 year)
        consensus.nMasternodePaymentsStartBlock = 65; // 65 blocks
        consensus.nMasternodePaymentsIncreaseBlock = 95; // 95 blocks
        consensus.nMasternodePaymentsIncreasePeriod = 30; // ~60 mins in blocks
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 255500; // one year from now
        consensus.nBudgetPaymentsCycleBlocks = 21000; // ~(60*24*30)/2, actual number of blocks per month is 255500 / 12 = 21291
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockStartBlock = 276500; // one year and 1 month from now
        consensus.nSuperblockCycle = 21000; // ~(60*24*30)/2, actual number of blocks per month is 255500 / 12 = 21291
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256(); // 0
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // GLYNO: 1 day
        consensus.nPowTargetSpacing = 2 * 60; // GLYNO: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1529028000; // Friday, June 15, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout =  1536976800; // September 15, 2018 4:00:00 AM

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1529028000; // Friday, June 4, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1544842800; // December 15, 2018 4:00:00 PM
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 3226; // 80% of 4032

        // The best chain should have at least this much work.
       consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000b0001a"); // 10

        // By default assume that the signatures in ancestors of this block are valid.
       consensus.defaultAssumeValid = uint256S("0x00000d63d1ed573a262752a4f56da531c58c2c43e77f3fa7cd988b0bf91c3d8e"); // 10

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xa4;
        pchMessageStart[1] = 0xbb;
        pchMessageStart[2] = 0xfe;
        pchMessageStart[3] = 0xc1;
        vAlertPubKey = ParseHex("04cb5eadda9e418eac5c1f32e7ff7eb621bf1485784b2ac22b6248f286e8495e3b89014b6901284d66d2a3c56b80e1649a4e9d05cb265383c3d8f270a774ed68aa");
        nDefaultPort = 7442;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 100000;


	genesis = CreateGenesisBlock(1529028000, 1142222, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

/*
// calculate Genesis Block
         // Reset genesis
         consensus.hashGenesisBlock = uint256S("0x");
         std::cout << std::string("Begin calculating Mainnet Genesis Block:\n");
         if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) {
             LogPrintf("Calculating Mainnet Genesis Block:\n");
             arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
             uint256 hash;
             genesis.nNonce = 0;
             // This will figure out a valid hash and Nonce if you're
             // creating a different genesis block:
             // uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
             // hashTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow).getuint256();
             // while (genesis.GetHash() > hashTarget)
             while (UintToArith256(genesis.GetHash()) > hashTarget)
             {
                 ++genesis.nNonce;
                 if (genesis.nNonce == 0)
                 {
                     LogPrintf("NONCE WRAPPED, incrementing time");
                     std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                     ++genesis.nTime;
                 }
                 if (genesis.nNonce % 10000 == 0)
                 {
                     LogPrintf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                     // std::cout << strNetworkID << " nonce: " << genesis.nNonce << " time: " << genesis.nTime << " hash: " << genesis.GetHash().ToString().c_str() << "\n";
                 }
             }
             std::cout << "Mainnet ---\n";
             std::cout << "  nonce: " << genesis.nNonce <<  "\n";
             std::cout << "   time: " << genesis.nTime << "\n";
             std::cout << "   hash: " << genesis.GetHash().ToString().c_str() << "\n";
             std::cout << "   merklehash: "  << genesis.hashMerkleRoot.ToString().c_str() << "\n";
             // Mainnet nonce: 1142222  time: 1529028000  hash: 00000d63d1ed573a262752a4f56da531c58c2c43e77f3fa7cd988b0bf91c3d8e
	     //   merklehash: 28ab1b7ad72353df3ff93c8cd0fcec4d619f2972b12fbbfa68e8809a37e37e10
         }
         std::cout << std::string("Finished calculating Mainnet Genesis Block:\n");
*/


        assert(consensus.hashGenesisBlock == uint256S("0x00000d63d1ed573a262752a4f56da531c58c2c43e77f3fa7cd988b0bf91c3d8e"));
        assert(genesis.hashMerkleRoot == uint256S("0x28ab1b7ad72353df3ff93c8cd0fcec4d619f2972b12fbbfa68e8809a37e37e10"));


        vSeeds.push_back(CDNSSeedData("glynocoin.org", "seed.glynocoin.org" ));


        // GLYNO addresses start with 'G'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38); // G
        // GLYNO script addresses start with 'L'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,48); // L
        // GLYNO private keys
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,78); // Y
        // GLYNO BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xA2)(0x4E).convert_to_container<std::vector<unsigned char> >();
        // GLYNO BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xBD)(0xA4).convert_to_container<std::vector<unsigned char> >();

        // GLYNO BIP44 coin type is '5' // Dash
        nExtCoinType = 5;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "048f161331e2cca64d031102c7da1c38445d1e6bd3ba29f3ca87c5f9b971bb7e58a48265aead5a94d6ea2191c38b4ef5c81015d5fab7853775bd9a809de2120306";

        /**
         * What makes a good checkpoint block?
         * + Is surrounded by blocks with reasonable timestamps
         *   (no blocks before with a timestamp after, none after with
         *    timestamp before)
         * + Contains no strange transactions
         */

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
           (  0, uint256S("0x00000d63d1ed573a262752a4f56da531c58c2c43e77f3fa7cd988b0bf91c3d8e")),
            1529028000, // * UNIX timestamp of last checkpoint block
            0,    // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            100        // * estimated number of transactions per day after checkpoint
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
        consensus.nSubsidyHalvingInterval = 255500;
        consensus.nMasternodePaymentsStartBlock = 5; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 15;
        consensus.nMasternodePaymentsIncreasePeriod = 20;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 200;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 250; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockCycle = 24; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // GLYNO: 1 day
        consensus.nPowTargetSpacing = 2 * 60; // GLYNO: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1529028000; //June 15, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout =  1536976800; // September 15, 2018 4:00:00 AM

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1529028000; // June 15, 2018 4:00:00 PM
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1544842800; //December 15, 2018 4:00:00 PM
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000600015"); // 5

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000d63d1ed573a262752a4f56da531c58c2c43e77f3fa7cd988b0bf91c3d8e"); // 5

        pchMessageStart[0] = 0xa4;
        pchMessageStart[1] = 0xbb;
        pchMessageStart[2] = 0xfe;
        pchMessageStart[3] = 0xc1;
        vAlertPubKey = ParseHex("04431a6c47e46533ea08dbcf72fc781ca64d6613f0da281d8ebe6324b27cd88fcb1b55d5a687e31f2b83b70e03376d5489e8460f25256104d3d3d37023671967b6");
        nDefaultPort = 17442;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1529028000, 1142222, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00000d63d1ed573a262752a4f56da531c58c2c43e77f3fa7cd988b0bf91c3d8e"));
        assert(genesis.hashMerkleRoot == uint256S("0x28ab1b7ad72353df3ff93c8cd0fcec4d619f2972b12fbbfa68e8809a37e37e10"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("glyno.org", "tseed.glyno.org" ));

        // Testnet GLYNO addresses start with 'G'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38); // m
        // Testnet GLYNO script addresses start with 'L'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,48); // L
        // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,244);
        // Testnet GLYNO BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x77)(0x7F).convert_to_container<std::vector<unsigned char> >();
        // Testnet GLYNO BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x84).convert_to_container<std::vector<unsigned char> >();

        // Testnet GLYNO BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "048f161331e2cca64d031102c7da1c38445d1e6bd3ba29f3ca87c5f9b971bb7e58a48265aead5a94d6ea2191c38b4ef5c81015d5fab7853775bd9a809de2120306";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (   0, uint256S("0x00000d63d1ed573a262752a4f56da531c58c2c43e77f3fa7cd988b0bf91c3d8e")),

            1529028000, // * UNIX timestamp of last checkpoint block
            0,       // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.01         // * estimated number of transactions per day after checkpoint
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
        consensus.nMasternodePaymentsStartBlock = 20;
        consensus.nMasternodePaymentsIncreaseBlock = 35;
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
        consensus.nPowTargetTimespan = 24 * 60 * 60; // GLYNO: 1 day
        consensus.nPowTargetSpacing = 2.5 * 60; // GLYNO: 2.5 minutes
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
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256();

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000d63d1ed573a262752a4f56da531c58c2c43e77f3fa7cd988b0bf91c3d8e");

	pchMessageStart[0] = 0xa4;
        pchMessageStart[1] = 0xbb;
        pchMessageStart[2] = 0xfe;
        pchMessageStart[3] = 0xc1;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 0; // never delay GETHEADERS in regtests
        nDefaultPort = 27442;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1529028000, 1142222, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

          assert(consensus.hashGenesisBlock == uint256S("0x00000d63d1ed573a262752a4f56da531c58c2c43e77f3fa7cd988b0bf91c3d8e"));
          assert(genesis.hashMerkleRoot == uint256S("0x28ab1b7ad72353df3ff93c8cd0fcec4d619f2972b12fbbfa68e8809a37e37e10"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x00000d63d1ed573a262752a4f56da531c58c2c43e77f3fa7cd988b0bf91c3d8e")),
            1529028000,
            0,
            100
        };
        // Regtest GLYNO addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        // Regtest GLYNO script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        // Regtest private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Regtest GLYNO BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest GLYNO BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Regtest GLYNO BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;
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
