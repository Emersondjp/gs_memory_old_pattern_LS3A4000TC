#ifndef GS_MEMORY_CLASS__HPP
#define GS_MEMORY_CLASS__HPP

class gs_memory{
  protected:
    virtual bool check();
  public:
    virtual bool operate();
    virtual void dump();
}; // End gs_memory

class gs_regfile_128x64_8sw6sr : public gs_memory{
  private:
    uint64_t mem[128];
  protected:
    uint64_t out0;
    uint64_t out1;
    uint64_t out2;
    uint64_t out3;
    uint64_t out4;
    uint64_t out5;
    bool check(
        const bool we0, const uint8_t wad0, const bool we1, const uint8_t wad1,
        const bool we2, const uint8_t wad2, const bool we3, const uint8_t wad3,
        const bool we4, const uint8_t wad4, const bool we5, const uint8_t wad5,
        const bool we6, const uint8_t wad6, const bool we7, const uint8_t wad7,
        const bool re0, const uint8_t rad0, const bool re1, const uint8_t rad1,
        const bool re2, const uint8_t rad2, const bool re3, const uint8_t rad3,
        const bool re4, const uint8_t rad4, const bool re5, const uint8_t rad5);
  public:
    gs_regfile_128x64_8sw6sr(const uint64_t val=0x00ull);
    bool operate(
        const bool we0, const uint8_t wad0, const uint64_t in0,
        const bool we1, const uint8_t wad1, const uint64_t in1,
        const bool we2, const uint8_t wad2, const uint64_t in2,
        const bool we3, const uint8_t wad3, const uint64_t in3,
        const bool we4, const uint8_t wad4, const uint64_t in4,
        const bool we5, const uint8_t wad5, const uint64_t in5,
        const bool we6, const uint8_t wad6, const uint64_t in6,
        const bool we7, const uint8_t wad7, const uint64_t in7,
        const bool re0, const uint8_t rad0, const bool re1, const uint8_t rad1,
        const bool re2, const uint8_t rad2, const bool re3, const uint8_t rad3,
        const bool re4, const uint8_t rad4, const bool re5, const uint8_t rad5);
    bool memset(const uint64_t val = 0x00ull);
    bool write();
    bool read(const uint8_t addr=0x00);
    uint64_t get_out0();
    uint64_t get_out1();
    uint64_t get_out2();
    uint64_t get_out3();
    uint64_t get_out4();
    uint64_t get_out5();
    void dump();
};// end of gs_regfile_128x64_8sw6sr

class gs_regfile_128x64_4sw4sr : public gs_memory{
  private:
    uint64_t mem[128];
  protected:
    uint64_t out0;
    uint64_t out1;
    uint64_t out2;
    uint64_t out3;
    bool check(
        const bool we0, const uint8_t wad0, const bool we1, const uint8_t wad1,
        const bool we2, const uint8_t wad2, const bool we3, const uint8_t wad3,
        const bool re0, const uint8_t rad0, const bool re1, const uint8_t rad1,
        const bool re2, const uint8_t rad2, const bool re3, const uint8_t rad3);
  public:
    gs_regfile_128x64_4sw4sr(const uint64_t val=0x00ull);
    bool operate(
        const bool we0, const uint8_t wad0, const uint64_t in0,
        const bool we1, const uint8_t wad1, const uint64_t in1,
        const bool we2, const uint8_t wad2, const uint64_t in2,
        const bool we3, const uint8_t wad3, const uint64_t in3,
        const bool re0, const uint8_t rad0, const bool re1, const uint8_t rad1,
        const bool re2, const uint8_t rad2, const bool re3, const uint8_t rad3);
    bool memset(const uint64_t val = 0x00ull);
    bool write();
    bool read(const uint8_t addr=0x00);
    uint64_t get_out0();
    uint64_t get_out1();
    uint64_t get_out2();
    uint64_t get_out3();
    void dump();
}; // end of gs_regfile_128x64_4sw4sr

class gs_cp0q_ram_64x128_3sw5sr : public gs_memory{
  private:
    uint64_t mem_H[64];
    uint64_t mem_L[64];
    uint32_t mask[64];
  protected:
    uint64_t out0_H, out0_L;
    uint64_t out1_H, out1_L;
    uint64_t out2_H, out2_L;
    uint64_t out3_H, out3_L;
    uint64_t out4_H, out4_L;
    bool check(
        const bool we0, const uint8_t wad0, const bool we1, const uint8_t wad1,
        const uint64_t we2,
        const bool re0, const uint8_t rad0, const bool re1, const uint8_t rad1,
        const bool re2, const uint8_t rad2, const bool re3, const uint8_t rad3,
        const bool re4, const uint8_t rad4);
  public:
    gs_cp0q_ram_64x128_3sw5sr(const uint16_t mask=0xffffull, const uint64_t val_L=0x00ull,const uint64_t val_H=0x00ull);
    bool operate(
        const bool we0, const uint8_t wad0, const uint64_t wvalue0_L, const uint64_t wvalue0_H, const uint32_t wmask0,
        const bool we1, const uint8_t wad1, const uint64_t wvalue1_L, const uint64_t wvalue1_H, const uint32_t wmask1,
        const uint64_t wen2,
        const bool re0, const uint8_t rad0, const bool re1, const uint8_t rad1,
        const bool re2, const uint8_t rad2, const bool re3, const uint8_t rad3,
        const bool re4, const uint8_t rad4);
    bool memset(const uint16_t mask=0xffffull, const uint64_t val_L=0x00ull, const uint64_t val_H=0x00ull);
    bool write();
    bool read(const uint8_t addr=0x00);
    uint64_t get_out0H();
    uint64_t get_out0L();
    uint64_t get_out1H();
    uint64_t get_out1L();
    uint64_t get_out2H();
    uint64_t get_out2L();
    uint64_t get_out3H();
    uint64_t get_out3L();
    uint64_t get_out4H();
    uint64_t get_out4L();
    void dump();
}; // end of gs_cp0q_ram_64x128_3sw5sr

class gs_cp0q_ram_48x64_2sw5sr : public gs_memory{
  private:
    uint64_t mem[48];
  protected:
    uint64_t out0;
    uint64_t out1;
    uint64_t out2;
    uint64_t out3;
    uint64_t out4;
    bool check( const bool we0, const uint8_t wad0, const bool we1, const uint8_t wad1,
        const bool re0, const uint8_t rad0, const bool re1, const uint8_t rad1, const bool re2, const uint8_t rad2,
        const bool re3, const uint8_t rad3, const bool re4, const uint8_t rad4);
  public:
    gs_cp0q_ram_48x64_2sw5sr(const uint64_t val=0x00ull);
    bool operate(
        const bool we0, const uint8_t wad0, const uint64_t in0,
        const bool we1, const uint8_t wad1, const uint64_t in1,
        const bool re0, const uint8_t rad0, const bool re1, const uint8_t rad1,
        const bool re2, const uint8_t rad2, const bool re3, const uint8_t rad3,
        const bool re4, const uint8_t rad4);
    bool memset(const uint64_t val=0x00ull);
    bool write();
    bool read(const uint8_t addr=0x00);
    uint64_t get_out0();
    uint64_t get_out1();
    uint64_t get_out2();
    uint64_t get_out3();
    uint64_t get_out4();
    void dump();
}; // end of gs_cp0q_ram_48x64_2sw5sr

class gs_cam_464v_64x64_1wrs : public gs_memory{
  private:
    uint64_t cam_vpn[64];  // 47:0
    uint32_t cam_mask[64]; // 17:0
    uint16_t cam_asid[64]; //  9:0
    bool     cam_g[64];
    uint64_t ram[64];
  protected:
    uint64_t out;
    uint64_t match;
    bool     hit;
    bool check(const bool se, const uint64_t svpn, const uint16_t sasid, const uint64_t valid,
        const bool we, const uint64_t wvpn, const uint32_t wmask, const uint16_t wasid,
        const bool re, const uint16_t addr);
  public:
    gs_cam_464v_64x64_1wrs(const uint64_t vpn=0x00ull, const uint32_t mask=0x00ul,
        const uint16_t asid=0x00, const bool g=false, const uint64_t data=0x00ull);
    bool operate(const bool se, const uint64_t svpn, const uint16_t sasid, const uint64_t valid,
        const bool we, const uint64_t wdata, const uint64_t wvpn,
        const uint32_t wmask, const uint16_t wasid, const bool g,
        const bool re, const uint16_t addr);
    bool memset(const uint64_t vpn=0x00ull, const uint32_t mask=0x00ul,
        const uint16_t asid=0x00, const bool g=false, const uint64_t data=0x00ull);
    bool write();
    bool read(const uint8_t addr=0x00);
    uint16_t get_asid(const uint8_t addr=0x00);
    uint64_t get_vpn(const uint8_t addr=0x00);
    uint64_t get_out();
    uint64_t get_match();
    bool     get_hit();
    void dump();
}; // end of gs_cam_464v_64x64_1wrs

class gs_cam_btb_30x96_1w1s : public gs_memory{
  private:
    uint32_t cam[96];  // 29:0
    uint64_t ram[96];  // 45:0
  protected:
    uint64_t out;  // 45:0
    uint32_t match31_00;
    uint32_t match63_32;
    uint32_t match95_64;
    bool hit;
    bool check( const bool se, const uint32_t svpn,
        const uint32_t valid31_00, const uint32_t valid63_32, const uint32_t valid95_64,
        const bool we, const uint32_t addr31_00, const uint32_t addr63_32,
        const uint32_t addr95_64, const uint32_t wvpn, const uint64_t data );
  public:
    gs_cam_btb_30x96_1w1s(const uint32_t vpn=0x00ul, const uint64_t data=0x00ull);
    bool operate( const bool se, const uint32_t svpn,
        const uint32_t valid31_00, const uint32_t valid63_32, const uint32_t valid95_64,
        const bool we, const uint32_t addr31_00, const uint32_t addr63_32,
        const uint32_t addr95_64, const uint64_t data, const uint32_t wvpn );
    bool memset(const uint32_t vpn=0x00ull, const uint64_t data=0x00ull);
    bool memset2(const uint32_t vpn=0x00ull, const uint64_t data=0x00ull);
    uint32_t get_svpn(const uint8_t addr=0x00);
    bool write();
    bool read(const uint8_t addr=0x00);
    uint64_t get_out();
    bool get_hit();
    uint32_t get_match31_00();
    uint32_t get_match63_32();
    uint32_t get_match95_64();
    void dump();
}; // end of gs_cam_btb_30x96_1w1s

#endif

