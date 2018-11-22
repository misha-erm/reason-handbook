module LoginForm = {
  open Formality;

  type field =
    | Email
    | Password;

  type state = {
    email: string,
    password: string,
  };

  type message = string;

  let passwordRegEx = Js.Re.fromString({|^.{4,8}$|});

  let validators = [
    {
      field: Email,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: state =>
        switch (state.email) {
        | str when String.length(str) > 4 => Ok(Valid)
        | _ => Error("Too short")
        },
    },
    {
      field: Password,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: state =>
        switch (state.password) {
        | str when str->Js.Re.test(passwordRegEx) => Ok(Valid)
        | _ => Error("Should be 4-8 chars length")
        },
    },
  ];
};

module FormContainer = Formality.Make(LoginForm);

let getFieldStatus = (result: option(Formality.result(string))) => {
    switch (result) {
    | Some(Error(message)) => ReasonReact.string(message)
    | Some(Ok(Valid)) => ReasonReact.string("valid")
    | Some(Ok(NoValue)) | None => ReasonReact.null
    };
};

let component = ReasonReact.statelessComponent("Form");

let make = _children => {
  ...component,
  render: _self =>
    <>
      <a href="https://github.com/alexfedoseev/re-formality">
        {ReasonReact.string("Using ReFormality lib")}
      </a>
      <FormContainer
        initialState={email: "", password: ""}
        onSubmit={
          (state, {notifyOnSuccess, reset}) => {
            Js.log2("Submitted with: ", state);
            Js.Global.setTimeout(
              () => {
                notifyOnSuccess(None);
                reset();
              },
              2000,
            )
            ->ignore;
          }
        }>
        ...{
             form =>
               <form onSubmit={form.submit->Formality.Dom.preventDefault}>
                 <div>
                    {ReasonReact.string("Email")}
                    <input
                    value={form.state.email}
                    disabled={form.submitting}
                    onBlur={_ => form.blur(Email)}
                    onChange={
                        event =>
                        form.change(
                            Email,
                            {
                            ...form.state,
                            email: ReactEvent.Form.target(event)##value,
                            },
                        )
                    }
                    />
                    (form.result(Email)->getFieldStatus)
                 </div>
                 <div>
                    {ReasonReact.string("Password")}
                    <input
                    value={form.state.password}
                    disabled={form.submitting}
                    onBlur={_ => form.blur(Password)}
                    onChange={
                        event =>
                        form.change(
                            Password,
                            {
                            ...form.state,
                            password: ReactEvent.Form.target(event)##value,
                            },
                        )
                    }
                    />
                    (form.result(Password)->getFieldStatus)
                 </div>
                 <div>
                    <button>
                    (form.submitting ? "Submitting..." : "Submit")
                    ->ReasonReact.string
                    </button>
                 </div>
               </form>
           }
      </FormContainer>
    </>,
};