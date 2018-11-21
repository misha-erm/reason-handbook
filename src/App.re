type route =
  | FormRoute
  | I18nRoute
  | CounterRoute
  | NotFoundRoute;

type state = {route};

type action =
  | ChangeRoute(route);

let reducer = (action, _state) =>
  switch (action) {
  | ChangeRoute(route) => ReasonReact.Update({route: route})
  };

let matchUrlToComponent = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | []
  | ["counter"] => CounterRoute
  | ["form"] => FormRoute
  | ["i18n"] => I18nRoute
  | _ => NotFoundRoute
  };

let changeRoute = (route, _event, self) =>
  self.ReasonReact.send(ChangeRoute(route));

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {
    route: ReasonReact.Router.dangerouslyGetInitialUrl()->matchUrlToComponent,
  },
  reducer,
  didMount: self => {
    let watchId =
      ReasonReact.Router.watchUrl(url =>
        url->matchUrlToComponent->ChangeRoute->(self.send)
      );
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watchId));
  },
  render: self =>
    <>
      <h2> {ReasonReact.string("ReasonReact Handbook")} </h2>
      <ul className="nav">
        <li onClick={_event => ReasonReact.Router.push("/counter")}>
          {ReasonReact.string("Counter")}
        </li>
        <li onClick={_event => ReasonReact.Router.push("/form")}>
          {ReasonReact.string("Form")}
        </li>
        <li onClick={_event => ReasonReact.Router.push("/i18n")}>
          {ReasonReact.string("I18n")}
        </li>
      </ul>
      <div>
        {
          switch (self.state.route) {
          | FormRoute => <Form />
          | I18nRoute => <div />
          | CounterRoute => <Counter />
          | NotFoundRoute => ReasonReact.string("NOT FOUND")
          }
        }
      </div>
    </>,
};