// Generated by view binder compiler. Do not edit!
package com.example.signbuddy.databinding;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.viewbinding.ViewBinding;
import androidx.viewbinding.ViewBindings;
import com.example.signbuddy.R;
import java.lang.NullPointerException;
import java.lang.Override;
import java.lang.String;

public final class ActivityMainBinding implements ViewBinding {
  @NonNull
  private final ConstraintLayout rootView;

  @NonNull
  public final Button connectButton;

  @NonNull
  public final ProgressBar connectProgress;

  @NonNull
  public final Button lessonButton;

  @NonNull
  public final ImageView logo;

  @NonNull
  public final Button quizButton;

  @NonNull
  public final TextView welcomeMsg;

  private ActivityMainBinding(@NonNull ConstraintLayout rootView, @NonNull Button connectButton,
      @NonNull ProgressBar connectProgress, @NonNull Button lessonButton, @NonNull ImageView logo,
      @NonNull Button quizButton, @NonNull TextView welcomeMsg) {
    this.rootView = rootView;
    this.connectButton = connectButton;
    this.connectProgress = connectProgress;
    this.lessonButton = lessonButton;
    this.logo = logo;
    this.quizButton = quizButton;
    this.welcomeMsg = welcomeMsg;
  }

  @Override
  @NonNull
  public ConstraintLayout getRoot() {
    return rootView;
  }

  @NonNull
  public static ActivityMainBinding inflate(@NonNull LayoutInflater inflater) {
    return inflate(inflater, null, false);
  }

  @NonNull
  public static ActivityMainBinding inflate(@NonNull LayoutInflater inflater,
      @Nullable ViewGroup parent, boolean attachToParent) {
    View root = inflater.inflate(R.layout.activity_main, parent, false);
    if (attachToParent) {
      parent.addView(root);
    }
    return bind(root);
  }

  @NonNull
  public static ActivityMainBinding bind(@NonNull View rootView) {
    // The body of this method is generated in a way you would not otherwise write.
    // This is done to optimize the compiled bytecode for size and performance.
    int id;
    missingId: {
      id = R.id.connectButton;
      Button connectButton = ViewBindings.findChildViewById(rootView, id);
      if (connectButton == null) {
        break missingId;
      }

      id = R.id.connectProgress;
      ProgressBar connectProgress = ViewBindings.findChildViewById(rootView, id);
      if (connectProgress == null) {
        break missingId;
      }

      id = R.id.lessonButton;
      Button lessonButton = ViewBindings.findChildViewById(rootView, id);
      if (lessonButton == null) {
        break missingId;
      }

      id = R.id.logo;
      ImageView logo = ViewBindings.findChildViewById(rootView, id);
      if (logo == null) {
        break missingId;
      }

      id = R.id.quizButton;
      Button quizButton = ViewBindings.findChildViewById(rootView, id);
      if (quizButton == null) {
        break missingId;
      }

      id = R.id.welcomeMsg;
      TextView welcomeMsg = ViewBindings.findChildViewById(rootView, id);
      if (welcomeMsg == null) {
        break missingId;
      }

      return new ActivityMainBinding((ConstraintLayout) rootView, connectButton, connectProgress,
          lessonButton, logo, quizButton, welcomeMsg);
    }
    String missingId = rootView.getResources().getResourceName(id);
    throw new NullPointerException("Missing required view with ID: ".concat(missingId));
  }
}