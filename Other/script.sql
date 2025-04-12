-- Bảng DiaChi
CREATE TABLE DiaChi (
    MaDC SERIAL PRIMARY KEY,
    TenDC TEXT NOT NULL
);

-- Bảng NguoiBan
CREATE TABLE NguoiBan (
    MaNB SERIAL PRIMARY KEY,
    Ten TEXT NOT NULL,
    MST TEXT,
    Sdt TEXT,
    Email TEXT,
    NgayDK DATE,
    MatKhau TEXT NOT NULL,
    MaDC INT REFERENCES DiaChi(MaDC)
);

-- Bảng PhuongThucThanhToan
CREATE TABLE PhuongThucThanhToan (
    MaPTTT SERIAL PRIMARY KEY,
    Ten TEXT NOT NULL,
    Loai TEXT
);

-- Bảng KhachHang
CREATE TABLE KhachHang (
    MaKH SERIAL PRIMARY KEY,
    MatKhau TEXT NOT NULL,
    Ten TEXT,
    Sdt TEXT,
    Email TEXT,
    NgayDK DATE,
    MaPTTT INT REFERENCES PhuongThucThanhToan(MaPTTT)
);

-- Bảng ChiTietDC
CREATE TABLE ChiTietDC (
    MaKH INT REFERENCES KhachHang(MaKH),
    MaDC INT REFERENCES DiaChi(MaDC),
    PRIMARY KEY (MaKH, MaDC)
);

-- Bảng ChiTietTT (Khách hàng - Phương thức thanh toán)
CREATE TABLE ChiTietTT (
    MaKH INT REFERENCES KhachHang(MaKH),
    MaPTTT INT REFERENCES PhuongThucThanhToan(MaPTTT),
    PRIMARY KEY (MaKH, MaPTTT)
);

-- Bảng PhuongThucVanChuyen
CREATE TABLE PhuongThucVanChuyen (
    MaPTVC SERIAL PRIMARY KEY,
    TenPTVC TEXT NOT NULL,
    Gia NUMERIC
);

-- Bảng PTVCNB (Người bán - Phương thức vận chuyển)
CREATE TABLE PTVCNB (
    MaNB INT REFERENCES NguoiBan(MaNB),
    MaPTVC INT REFERENCES PhuongThucVanChuyen(MaPTVC),
    PRIMARY KEY (MaNB, MaPTVC)
);

-- Bảng Voucher
CREATE TABLE Voucher (
    MaVC SERIAL PRIMARY KEY,
    TenVC TEXT NOT NULL,
    GiaTri NUMERIC,
    SoLuong INT,
    NgayHetHan DATE,
    NgayKhaDung DATE
);

-- Bảng VoucherNB
CREATE TABLE VoucherNB (
    MaVC INT REFERENCES Voucher(MaVC),
    MaNB INT REFERENCES NguoiBan(MaNB),
    TenVC TEXT,
    GiaTri NUMERIC,
    SoLuong INT,
    NgayHetHan DATE,
    PRIMARY KEY (MaVC)
);

-- Bảng ViVoucher
CREATE TABLE ViVoucher (
    MaVi SERIAL PRIMARY KEY,
    MaKH INT REFERENCES KhachHang(MaKH),
    MaVC INT REFERENCES Voucher(MaVC)
);

-- Bảng SanPham
CREATE TABLE SanPham (
    MaSP SERIAL PRIMARY KEY,
    Ten TEXT NOT NULL,
    NgayNhapKho DATE,
    SoLuong INT,
    GiaBan NUMERIC,
    MaNB INT REFERENCES NguoiBan(MaNB)
);

-- Bảng DonHang
CREATE TABLE DonHang (
    MaDH SERIAL PRIMARY KEY,
    NgayDat DATE,
    TrangThai TEXT,
    MaDC INT REFERENCES DiaChi(MaDC),
    MaKH INT REFERENCES KhachHang(MaKH),
    MaPTTT INT REFERENCES PhuongThucThanhToan(MaPTTT),
    TrangThaiThanhToan TEXT,
    MaVC INT REFERENCES Voucher(MaVC),
    MaVCNB INT REFERENCES VoucherNB(MaVC),
    MaPTVC INT REFERENCES PhuongThucVanChuyen(MaPTVC),
    TongTien NUMERIC
);

-- Bảng ChiTietDonHang
CREATE TABLE ChiTietDonHang (
    MaDH INT REFERENCES DonHang(MaDH),
    MaSP INT REFERENCES SanPham(MaSP),
    SoLuong INT,
    PRIMARY KEY (MaDH, MaSP)
);
