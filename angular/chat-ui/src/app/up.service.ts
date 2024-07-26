import { Injectable } from '@angular/core';
import { MatDialog, MatDialogConfig } from '@angular/material/dialog';
import { MatSnackBar } from '@angular/material/snack-bar';

import { SystemMaintenanceComponent } from './system-maintenance/system-maintenance.component';

@Injectable({
  providedIn: 'root'
})
export class UpService {

  constructor(
    private dialog: MatDialog,
    private snackBar: MatSnackBar,
  ) { }

  private _up = true;
  private dlg: any = null;

  public isUp(): boolean {
    return this._up;
  }

  public down() {
    if (this._up) {
      this._up = false;
    }
  }

  public up() {
    if (!this._up) {
      this._up = true;
    }
  }

  public disconnected() {
    this.down();
    this.showMaintenanceDialog();
  }

  public reconnecting() {
    this.snackBar.open("Server unavailable, attempting to reconnect.");
  }

  public reconnected(reload: boolean) {
    this.closeDialog();
    if (reload) {
      window.location.reload();
    }
  }

  public connected() {
      this.snackBar.dismiss();
  }

  private showMaintenanceDialog() {
    if (!this.dlg) {
      const dialogConfig = new MatDialogConfig();
      dialogConfig.disableClose = true;
      dialogConfig.autoFocus = true;
      this.dlg = this.dialog.open(SystemMaintenanceComponent, dialogConfig);
    }
  }

  public closeDialog() {
    if (this.dlg) {
      this.dlg.close();
      this.dlg = null;
    }
  }

}
