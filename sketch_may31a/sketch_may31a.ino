+import cv2
import os
import math
from PIL import Image
NUMPIXELS = 32 
Div = 200

gif_file_name = "magic.gif"
im = Image.open(gif_file_name)
print(im.is_animated)

print(im.n_frames)
Frame=im.n_frames

gif = cv2.VideoCapture(gif_file_name)

 

file = open('graphics.h', 'w')
file.write('#define Frame ' + str(Frame) + '\n')
file.write('#define NUMPIXELS ' + str(NUMPIXELS) + '\n')
file.write('#define Div ' + str(Div) + '\n' + '\n')

 
 
file.write('const uint32_t pic [Frame][Div][NUMPIXELS] = {' + '\n')
 

 
 

def polarConv(pic, i):
    imgOrgin = cv2.imread(pic) 
    
    h, w, _ = imgOrgin.shape 
 
   
    imgRedu = cv2.resize(imgOrgin,(math.floor((NUMPIXELS * 2 -1)/h *w), NUMPIXELS * 2 -1))
 
 
    
    h2, w2, _ = imgRedu.shape
    wC = math.floor(w2 / 2)
    hC = math.floor(h2 / 2)
 
    
    imgPolar = Image.new('RGB', (NUMPIXELS, Div))
 
 
   
    file.write('\t{\n')
    for j in range(0, Div):
        file.write('\t\t{')
        for i in range(0, hC+1):
           
            rP = int(imgRedu[hC + math.ceil(i * math.cos(2*math.pi/Div*j)),
                         wC - math.ceil(i * math.sin(2*math.pi/Div*j)), 2])
                     
            gP = int(imgRedu[hC + math.ceil(i * math.cos(2*math.pi/Div*j)),
                         wC - math.ceil(i * math.sin(2*math.pi/Div*j)), 1])
                     
            bP = int(imgRedu[hC + math.ceil(i * math.cos(2*math.pi/Div*j)),
                         wC - math.ceil(i * math.sin(2*math.pi/Div*j)), 0])
            
            file.write('0x%02X%02X%02X' % (rP,gP,bP))
            
            if i == hC:
                file.write('},\n')
            else:
                file.write(', ')
                
            imgPolar.putpixel((i,j), (rP, gP, bP))
                     
    file.write('\t},\n\n')
 
 

dir_name = "screen_caps"
if not os.path.exists(dir_name):
    os.mkdir(dir_name)
 

    
    
for i in range(Frame):
    is_success, frame = gif.read()
  
    img_name = str(i) + ".png"
    img_path = os.path.join(dir_name, img_name)
    
    cv2.imwrite(img_path, frame)
   
    polarConv(img_path, i)
    
 
 
file.write('};' + '\n' + '\n')
file.close()
