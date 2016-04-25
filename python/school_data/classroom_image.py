import Image
import ImageFile
import os
import base64

class ImageData(object):
    def __init__(self):
        pass
    def getImageData(self,classroom_name):
        file_name = '/home/liao/Documents/classroom_info/%s.png'%classroom_name
        has_pic = self.hasFile(file_name)
        if has_pic==False:    #this file is not exists
            return 'False'
        image = open(file_name,'rb')
        image_data = base64.b64encode(image.read()) #encode this image to base64
        return image_data

    def hasFile(self,file_name):    #check is this file exists
        if os.path.isfile(file_name):
            return True
        else:
            return False


#test
if __name__ == '__main__':
    name = 'D101'
    b = ImageData()
    a = b.getImageData(name)
    print a