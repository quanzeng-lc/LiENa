

class LienaControlInstruction():
    def __init__(self):
        self.guidewireTranslationalSpeed = 0
        self.guidewireRotationalSpeed = 0
        self.catheterTranslationalSpeed = 0

    def set_guidewire_translational_speed(self, v):
        self.guidewireTranslationalSpeed = v

    def set_guidewire_rotational_speed(self, v):
        self.guidewireRotationalSpeed = v

    def set_catheter_translational_speed(self, v):
        self.catheterTranslationalSpeed = v

    def get_guidewire_translational_speed(self):
        return self.guidewireTranslationalSpeed

    def get_guidewire_rotational_speed(self):
        return self.guidewireRotationalSpeed

    def get_catheter_translational_speed(self):
        return self.catheterTranslationalSpeed
