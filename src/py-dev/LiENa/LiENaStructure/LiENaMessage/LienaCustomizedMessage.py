from LiENa.LiENaStructure.LiENaMessage.LienaMessage import LienaMessage


class LienaCustomizedMessage(LienaMessage):
    def __init__(self, message_id, target_id, origin_id, timestamps, dlc):
        LienaMessage.__init__(self, message_id, target_id, origin_id, timestamps, dlc)
        self.index = 0
        self.rule = []
        self.v = bytearray()

    def define_body_length(self, l):
        for i in range(l):
            self.append_uint8(0)

    def configure(self, rule):
        self.rule = rule

    def get_message_body(self):
        return self.v

    # for decoding
    def pop_uint8(self):
        v = self.v[self.index]
        self.index += 1
        return v

    def pop_uint16(self):
        v1 = self.v[self.index]
        v2 = self.v[self.index + 1]
        self.index += 2
        return v1*256 + v2

    def pop_uint32(self):
        v1 = self.v[self.index]
        v2 = self.v[self.index + 1]
        v3 = self.v[self.index + 2]
        v4 = self.v[self.index + 3]
        self.index += 4
        return v1*256*256*256 + v2*256*256 + v3*256 + v4

    def pop_uint64(self):
        v1 = self.v[self.index]
        v2 = self.v[self.index + 1]
        v3 = self.v[self.index + 2]
        v4 = self.v[self.index + 3]
        v5 = self.v[self.index + 4]
        v6 = self.v[self.index + 5]
        v7 = self.v[self.index + 6]
        v8 = self.v[self.index + 7]
        self.index += 8
        return v1*256*256*256*256*256*256*256 + v2*256*256*256*256*256*256 + v3*256*256*256*256*256 + v4*256*256*256*256 + v5**256*256*256 + v6**256*256 + v7*256 + v8

    # for encoding
    def append_uint8(self, v):
        self.v[self.index] = v
        self.index += 1

    def append_uint16(self, v):
        v0 = v & 0xff00 >> 8
        v1 = v & 0x00ff
        self.v[self.index] = v0
        self.v[self.index+1] = v1
        self.index += 2

    def append_uint32(self, v):
        v0 = v & 0xff000000 >> 24
        v1 = v & 0x00ff0000 >> 16
        v2 = v & 0x0000ff00 >> 8
        v3 = v & 0x000000ff
        self.v.append(v0)
        self.v.append(v1)
        self.v.append(v2)
        self.v.append(v3)

    def append_uint64(self, v):
        v0 = v & 0xff00000000000000 >> 56
        v1 = v & 0x00ff000000000000 >> 48
        v2 = v & 0x0000ff0000000000 >> 40
        v3 = v & 0x000000ff00000000 >> 32
        v4 = v & 0x00000000ff000000 >> 24
        v5 = v & 0x0000000000ff0000 >> 16
        v6 = v & 0x000000000000ff00 >> 8
        v7 = v & 0x00000000000000ff
        self.v.append(v0)
        self.v.append(v1)
        self.v.append(v2)
        self.v.append(v3)
        self.v.append(v4)
        self.v.append(v5)
        self.v.append(v6)
        self.v.append(v7)

    def append_uint8_3d_point_cloud(self, pts):
        for pt in pts:
            self.append_uint8_3d_point(pt[0], pt[1], pt[2])

    def append_uint16_3d_point_cloud(self, pts):
        for pt in pts:
            self.append_uint16_3d_point(pt[0], pt[1], pt[2])

    def append_uint8_2d_point(self, x, y):
        self.append_uint8(x)
        self.append_uint8(y)

    def append_uint16_2d_point(self, x, y):
        self.append_uint16(x)
        self.append_uint16(y)

    def append_uint8_3d_point(self, x, y, z):
        self.append_uint8(x)
        self.append_uint8(y)
        self.append_uint8(z)

    def append_uint8_3d_point(self, x, y, z):
        self.append_uint16(x)
        self.append_uint16(y)
        self.append_uint16(z)

    def append_uint16_2d_point(self, x, y):
        self.append_uint16(x)
        self.append_uint16(y)

    def append_uint8_matrix(self, v):
        (w, h) = v.shape
        for i in range(w):
            for j in range(h):
                self.append_uint8(v[i][j])

    def append_uint16_matrix(self, v):
        (w, h) = v.shape
        for i in range(w):
            for j in range(h):
                self.append_uint16(v[i][j])

    def append_bool(self, v):
        if v:
            self.v.append(1)
        else:
            self.v.append(0)

    def append_int(self, v):
        if v >= 0:
            self.v.append(1)  # 1 means positive
            self.append_uint32(abs(v))
        else:
            self.v.append(0)  # 0 means negative
            self.append_uint32(abs(v))

    def append_long(self, v):
        if v >= 0:
            self.v.append(1)  # 1 means positive
            self.append_uint64(abs(v))
        else:
            self.v.append(0)  # 0 means negative
            self.append_uint64(abs(v))
