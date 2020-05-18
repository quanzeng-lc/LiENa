

class LienaControlInstruction():
    def __init__(self):
        self.guidewireTranslationalSpeed = 0
        self.guidewireRotationalSpeed = 0

        self.catheterTranslationalSpeed = 0

        self.contrastMediaVolume = 0
        self.contrastMediaSpeed = 0

    def set_guidewire_translational_speed(self, v):
        self.guidewireTranslationalSpeed = v

    def set_guidewire_rotational_speed(self, v):
        self.guidewireRotationalSpeed = v

    def set_catheter_translational_speed(self, v):
        self.catheterTranslationalSpeed = v

    def set_contrast_media_volume(self, v):
        self.contrastMediaVolume = v

    def set_contrast_media_speed(self, v):
        self.contrastMediaSpeed = v

    def get_guidewire_translational_speed(self):
        return self.guidewireTranslationalSpeed

    def get_guidewire_rotational_speed(self):
        return self.guidewireRotationalSpeed

    def get_catheter_translational_speed(self):
        return self.catheterTranslationalSpeed

    def get_contrast_media_volume(self):
        return self.contrastMediaVolume

    def get_contrast_media_speed(self):
        return self.contrastMediaSpeed
