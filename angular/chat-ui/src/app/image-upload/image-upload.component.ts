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

          // convert to a blob for upload.
          this.canvas.nativeElement.toBlob((blob) => {
            this.images.push(blob);
          }, this.files[i].type);
          // convert to an URL for display.
          let url = this.canvas.nativeElement.toDataURL(this.files[i].type);
          this.imageUrls.push(url);
        };
        img.src = event.target.result;
      }
      reader.readAsDataURL(this.files[i]);
    }
  }

  upload(): void {

    // upload all the files.
    const formData = new FormData();
    for (let i in this.files) {
      formData.append("files", this.images[i], this.files[i].name);
    }
    this.inProgress = true;
    this.mediaService.uploadNewMedia(this.id, "IMAGE", formData).pipe(
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
       return of(`upload failed.`);
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
