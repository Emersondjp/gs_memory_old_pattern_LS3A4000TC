#ifndef __TYPES_H__
#define __TYPES_H__
typedef union tb_testcfg_reg{
        uint32_t d32;
        struct {
                unsigned ncyc:10;
                unsigned reserved_15_10:6;
                unsigned tid:8;
                unsigned resreved_31_24:8;
        } b;
}tb_testcfg_reg_t;
typedef union tb_teststs_reg{
        uint32_t d32;
        struct {
                unsigned ecyc:10;
                unsigned reserved_15_10:6;
                unsigned ecnt:11;
                unsigned reserved_30_27:4;
                unsigned err:1;
        } b;
}tb_teststs_reg_t;
typedef union tb_testctl_reg{
        uint32_t d32;
        struct {
                unsigned start:1;
                unsigned reserved_31_1:31;
        } b;
}tb_testctl_reg_t;
typedef union tb_patbase_reg{
        uint32_t d32;
        struct {
                unsigned pat_base:10;
                unsigned reserved_15_10:6;
                unsigned gld_base:10;
                unsigned reserved_31_26:6;
        } b;
}tb_patbase_reg_t;

typedef struct testbed_regs{
        tb_testcfg_reg_t testcfg;
        tb_teststs_reg_t teststs;
        tb_testctl_reg_t testctl;
        tb_patbase_reg_t patbase;
}testbed_regs_t;

typedef struct rf8w6r_in {
        uint64_t cmp_mask;
        uint8_t  waddr[8];//6:0
        uint8_t  raddr[6];//6:0
        uint8_t  wen7_0;
        uint8_t  ren5_0;
        uint64_t d[8];
}tb_rf8w6r_in_t;

typedef struct rf8w6r_out{
        uint64_t q[6];
}tb_rf8w6r_out_t;

typedef struct rf4w4r_in {
        uint64_t cmp_mask;
        uint8_t waddr[4];//6:0
        uint8_t raddr[4];//6:0
        uint8_t wen3_0;
        uint8_t ren3_0;
        uint8_t dmy[6];
        uint64_t d[4];
}tb_rf4w4r_in_t;

typedef struct rf4w4r_out{
        uint64_t q[4];
        uint64_t q3_2[2];
}tb_rf4w4r_out_t;

typedef struct btbcam_in {
        uint64_t cmp_mask;
        uint32_t valid[3];
        uint32_t addr[3];
        uint64_t wd;//45:0
        uint32_t wvpn;//29:0
        uint32_t svpn;//29:0
        uint8_t  wen;//0
        uint8_t  sen;//0
}tb_btbcam_in_t;

typedef struct btbcam_out{
        uint32_t match[3];
        uint32_t rd[2];//45:0
        uint8_t  hit;//0
}tb_btbcam_out_t;

typedef struct cam64x64_in {
        uint64_t cmp_mask;
        uint64_t valid;
        uint64_t wd;
        uint64_t wvpn;//47:0
        uint64_t svpn;//47:0
        uint32_t wasid;//9:0
        uint32_t sasid;//9:0
        uint8_t  wen;//0
        uint8_t  ren;//0
        uint8_t  sen;//0
        uint8_t  addr;//5:0
        uint32_t mask;//17:0
        uint8_t  G;//0
}tb_cam64x64_in_t;

typedef struct cam64x64_out{
        uint64_t match;
        uint64_t rd;
        uint8_t  hit;//0
}tb_cam64x64_out_t;

typedef struct cp0q_ram_in {
        uint64_t cmp_mask;
        uint64_t waddr0;//47:0
        uint64_t wvalue0;
        uint64_t waddr1;//47:0
        uint64_t wvalue1;
        uint64_t raddr0;//47:0
        uint64_t raddr1;//47:0
        uint64_t raddr2;//47:0
        uint64_t raddr3;//47:0
        uint64_t raddr4;//47:0
        uint8_t wen1_0;
        uint8_t ren4_0;
}tb_cp0q_ram_in_t;

typedef struct cp0q_ram_out{
        uint64_t q0;
        uint64_t q1;
        uint64_t q2;
        uint64_t q3;
        uint64_t q4;
}tb_cp0q_ram_out_t;

typedef struct cp0q_ram_old_in {
        uint64_t cmp_mask;
        uint64_t wvalue0[2];
        uint64_t wvalue1[2];
        uint8_t  waddr0;//index: [5:0]  , is_zero: [6]
        uint8_t  waddr1;//index: [5:0]  , is_zero: [6]
        uint8_t  raddr0;//index: [5:0]  , is_zero: [6]
        uint8_t  raddr1;//index: [5:0]  , is_zero: [6]
        uint8_t  raddr2;//index: [5:0]  , is_zero: [6]
        uint8_t  raddr3;//index: [5:0]  , is_zero: [6]
        uint8_t  raddr4;//index: [5:0]  , is_zero: [6]
        uint8_t  ren;//[4:0]
        uint32_t wmask0;//mask: [15:0]
        uint32_t wmask1;//mask: [15:0] 
        uint8_t  ren2_bypass; 
        uint8_t  ren3_bypass;
        uint8_t  osel;
        uint8_t  tbypass0;
        uint8_t  tbypass1;
        uint8_t  tin0;
        uint8_t  tin1;
        uint8_t  tin2;
        uint64_t wen2;
}tb_cp0q_ram_old_in_t;

typedef struct cp0q_ram_old_out{
        uint64_t q0;
        uint64_t q1;
        uint64_t q2;
        uint64_t q3;
        uint64_t q4;
}tb_cp0q_ram_old_out_t;

typedef struct rf4w2r_in {
        uint64_t cmp_mask;
        uint32_t wen0;
        uint32_t wen1;
        uint32_t wen2;
        uint32_t wen3;
        uint32_t ren0;
        uint32_t ren1;
        uint32_t d0[2];
        uint32_t d1[2];
        uint32_t d2[2];
        uint32_t d3[2];
}tb_rf4w2r_in_t;

typedef struct rf4w2r_out{
        uint64_t q0;
        uint64_t q1;
}tb_rf4w2r_out_t;

typedef struct pat_item {
        uint32_t d32[32];
}pat_item_t;

typedef struct gld_item {
        uint32_t d32[16];
}gld_item_t;

typedef union tb_pat_item{
        pat_item_t data;
        tb_rf8w6r_in_t rf8w6r_in_item;
        tb_rf4w4r_in_t rf4w4r_in_item;
        tb_btbcam_in_t btbcam_in_item;
        tb_cam64x64_in_t cam64x64_in_item;
        tb_cp0q_ram_in_t cp0q_ram_in_item;
        tb_cp0q_ram_old_in_t cp0q_ram_old_in_item;
        tb_rf4w2r_in_t rf4w2r_in_item;
}tb_pat_item_t;

typedef union tb_gld_item{
        gld_item_t data;
        tb_rf8w6r_out_t rf8w6r_out_item;
        tb_rf4w4r_out_t rf4w4r_out_item;
        tb_btbcam_out_t btbcam_out_item;
        tb_cam64x64_out_t cam64x64_out_item;
        tb_cp0q_ram_out_t cp0q_ram_out_item;
        tb_cp0q_ram_old_out_t cp0q_ram_old_out_item;
        tb_rf4w2r_out_t rf4w2r_out_item;
}tb_gld_item_t;

typedef union clkcfg_reg{
        uint32_t d32;
        struct {
                unsigned pdn:1;
                unsigned eno:1;
                unsigned bypass:1; // only for gs_pll
                unsigned sel:1;    // 0: gs_pll
                unsigned ldf:12;
                unsigned odf:3;
                unsigned resreved_19:1;
                unsigned idf:3;
                unsigned resreved_23:1;
                unsigned busdiv:2;
                unsigned cpudiv:2;
                unsigned tbdiv:2;
                unsigned resreved_30:1;
                unsigned lock:1;
        } b;
}clkcfg_reg_t;

#endif
