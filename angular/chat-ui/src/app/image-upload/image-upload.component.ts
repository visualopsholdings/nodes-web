import { Component, ViewChild, ElementRef } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { catchError, map, tap } from 'rxjs/operators';
import { HttpEventType, HttpErrorResponse } from '@angular/common/http';
import { of } from 'rxjs';

import { MediaService }  from '../media.service';

@Component({
  selector: 'app-image-upload',
  templateUrl: './image-upload.component.html',
  styleUrls: ['./image-upload.component.css']
})
export class ImageUploadComponent {

  @ViewChild('canvas') canvas: ElementRef<HTMLCanvasElement>;
  @ViewChild("preview") preview: ElementRef;
  @ViewChild("fileUpload") fileUpload: ElementRef;

  files: File[] = [];
  images: any[];
  imageUrls: string[];
  maxWidth = 128;
  file: any;
  id: string;
  inProgress = false;
  progress = 0;
  noThumbnail = null;

  constructor(
    private router: Router,
    private route: ActivatedRoute,
    private mediaService: MediaService,
  ) {

    route.params.subscribe(params => {
      this.id = params["id"];
    });
  }

  choose(): void {

    const input = this.fileUpload.nativeElement;

    this.images = [];
    this.files = [];

    if (!input.files?.length) {
        return;
    }

    // copy the files ready.
    for (let i=0; i<input.files.length; i++) {
      this.files.push(input.files[i]);
    }

    // render all the previews
    this.loadImages();

  }

  loadImages(): void {

    this.images = [];
    this.imageUrls = [];

    for (let i in this.files) {
      let reader = new FileReader();
      reader.onload = (event: any) => {
        const img = new Image();
        img.onload = () => {
          const ctx = this.canvas.nativeElement.getContext('2d');

          let newWidth = img.width;
          let newHeight = img.height;

          if (newWidth > this.maxWidth) {
              newHeight = (img.height * this.maxWidth) / img.width;
              newWidth = this.maxWidth;
          }

          // Set canvas dimensions
          this.canvas.nativeElement.width = newWidth;
          this.canvas.nativeElement.height = newHeight;

          ctx.drawImage(img, 0, 0, newWidth, newHeight);

          this.canvas.nativeElement.toBlob((blob) => {
            this.images.push(blob);
          }, 'image/jpeg', 0.8);
          let url = this.canvas.nativeElement.toDataURL('image/jpeg', 0.8);
          this.imageUrls.push(url);
        };
        img.src = event.target.result;
      }
      reader.readAsDataURL(this.files[i]);
    }
  }

  upload(): void {

    // upload the first image.
    let data = this.images[0];
    let file = this.files[0];

    const formData = new FormData();
    formData.append('file', data, "resized.jpeg");
    this.inProgress = true;
    this.mediaService.uploadNewMedia(formData).pipe(
      map(event => {
        switch (event.type) {
          case HttpEventType.UploadProgress:
            this.progress = Math.round(event.loaded * 100 / event.total);
            break;
          case HttpEventType.Response:
            return event;
        }
      }),
      catchError((error: HttpErrorResponse) => {
        this.inProgress = false;
//        uploaded(false);
//        element.value = null;
       return of(`${file.name} upload failed.`);
      })).subscribe((event: any) => {
        if (event && event.status == 200) {
          this.progress = 100;
          this.inProgress = false;
          this.noThumbnail = "Image upload successful.";
//          uploaded(true);
//           element.value = event.body._id;
//           this.textChange(element);
        }
      });
  }

}
